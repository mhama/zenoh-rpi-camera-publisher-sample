#include <chrono>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <opencv2/opencv.hpp> // OpenCV
#include "zenoh.hxx"          // zenoh-cpp

using namespace zenoh;
using namespace std::chrono_literals;

// デフォルト値
static const char *default_keyexpr = "rpi/camera/image_jpeg";

int _main(int argc, char **argv) {

    auto config = Config::create_default();

    // 送信先のキーを取得
    auto keyexpr = default_keyexpr;

    // Zenoh セッションを開く
    std::cout << "Opening Zenoh session..." << std::endl;
    auto session = Session::open(std::move(config));

    // Publisher を宣言
    std::cout << "Declaring Publisher on '" << keyexpr << "'..." << std::endl;
    auto pub = session.declare_publisher(KeyExpr(keyexpr));

    // libcamerasrcからだと流し込める。
    // camera-name は libcamera-hello --list-camera の結果の括弧 () 内の文字列
    // 0: "/base/axi/pcie@120000/rp1/i2c@88000/imx708@1a"
    // 1: "/base/axi/pcie@120000/rp1/i2c@80000/imx708@1a"
    std::string pipeline =
        "libcamerasrc camera-name=\"/base/axi/pcie@120000/rp1/i2c@88000/imx708@1a\" "
	"! video/x-raw,format=RGBx,width=640,height=480,framerate=30/1 "
        "! videoconvert "
        "! appsink";

    std::cout << "opening camera..." << std::endl;
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    // initializer for webcams
    //cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Failed to open webcam!" << std::endl;
        return -1;
    }

    const int delay_ms = 1;

    std::cout << "Start capturing and publishing..." << std::endl;
    std::cout << "Press CTRL-C to quit..." << std::endl;


    std::vector<int> param = std::vector<int>(2);
    param[0]=cv::IMWRITE_JPEG_QUALITY;
    param[1]=85;

    // メインループ
    while (true) {
        cv::Mat frame;
        cap >> frame; // カメラからフレーム取得
        if (frame.empty()) {
            std::cerr << "Captured empty frame. Continue..." << std::endl;
            continue;
        }

        // JPEG 形式でエンコード
        std::vector<uchar> encoded;
        if (!cv::imencode(".jpg", frame, encoded, param)) {
            std::cerr << "Failed to encode frame as JPEG. Continue..." << std::endl;
            continue;
        }

        // バイト列を文字列として送信
        Bytes payload = std::move(encoded);

        // Zenoh で送信
        Publisher::PutOptions options; // 必要に応じてオプションを設定
        pub.put(payload.clone());//, std::move(options));
	std::cout << "sent a frame." << std::endl;

	// wait a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    return 0;
}

int main(int argc, char **argv) {
    try {
#ifdef ZENOHCXX_ZENOHC
        init_log_from_env_or("error");
#endif
        return _main(argc, argv);
    } catch (ZException &e) {
        std::cout << "Received an error: " << e.what() << "\n";
        return -1;
    }
}

