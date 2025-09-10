cmake_minimum_required(VERSION 3.10)
project(sensor_fusion_headtracking)

set(CMAKE_CXX_STANDARD 17)

# === Python finden ===
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# === Include directories ===
include_directories(
    include
    external  # hier liegt matplotlibcpp.h
    ${Python3_INCLUDE_DIRS}  # Python Header
)

# === Quellen ===
add_executable(main
    src/main.cpp
    src/i2c_mpu9250.cpp
    src/tools.cpp
)

# === Libraries ===
target_link_libraries(main
    i2c
    ${Python3_LIBRARIES}  # gegen Python linken
)
