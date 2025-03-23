# camera image publisher for Raspberry Pi 5 CSI camera

# How to use

## install gstreamer

```sh
sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-gl gstreamer1.0-plugins-base-apps libcamera-dev gstreamer1.0-libcamera
```

## install opencv library

```sh
sudo apt install libopencv-dev
```

## install rust toolchain

```sh
sudo apt install cargo cmake
```

## build and install zenoh-c

```sh
git clone https://github.com/eclipse-zenoh/zenoh-c.git

cd zenoh-c
mkdir -p build && cd build 
cmake ..
cmake --build . --config Release
```

install

```sh
sudo cmake --build . --target install
```

## build and install zenoh-cpp

follow instruction of https://github.com/eclipse-zenoh/zenoh-cpp

actually:

```sh
git clone https://github.com/eclipse-zenoh/zenoh-cpp.git
cd zenoh-cpp
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=~/.local
cmake --install .
```

## cmake

```sh
mkdir build && cd build
cmake ..
```

## make

```sh
make Main
```

# How to run

```sh
./Main
```


