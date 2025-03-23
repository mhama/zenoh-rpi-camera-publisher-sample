cmake_minimum_required(VERSION 3.16)
project(z_pub_camera)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "~/.local")

add_executable(Main z_pub_camera.cpp)

# zenoh-cpp
find_package(zenohc REQUIRED)
find_package(zenohcxx REQUIRED)

# OpenCV
find_package(OpenCV REQUIRED)
target_include_directories(Main PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(Main PUBLIC ${OpenCV_LIBS} zenohcxx::zenohc)

