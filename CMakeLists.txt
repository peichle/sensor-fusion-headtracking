cmake_minimum_required(VERSION 3.10)
project(sensor_fusion_headtracking)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(main
    src/main.cpp
    src/i2c_mpu9250.cpp
    #src/orientation.cpp
    #src/sensor_fusion.cpp
    #src/udp_sender.cpp
)

# libi2c separat einbinden:
target_link_libraries(main i2c)