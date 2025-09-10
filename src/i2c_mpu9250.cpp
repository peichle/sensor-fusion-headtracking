#include "../include/i2c_mpu9250.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <iostream>

#include "../external/matplotlibcpp.h"

namespace plt = matplotlibcpp;

extern "C" {
    #include <i2c/smbus.h>
}

constexpr uint8_t PWR_MGMT_1  = 0x6B;

constexpr float ACCEL_SCALE = 16384.0f;
constexpr float GYRO_SCALE = 131.0f;
constexpr float MAGNETO_SCALE = 0.6f; // 0.6 LSB according to sensor specification
constexpr float GRAVITY_MS2 = 9.80665f;

constexpr uint8_t ACCEL_XOUT_H  = 0x3B;
constexpr uint8_t GYRO_XOUT_H   = 0x43;

constexpr uint8_t HXL = 0x03;
constexpr uint8_t HYL = 0x05;
constexpr uint8_t HZL = 0x07;

// Max. Anzahl an Punkten im Plot
constexpr int MAX_POINTS = 100;

MPU9250::MPU9250(const char* device, int address)
    : device_(device), address_(address), file_(-1) {}

MPU9250::~MPU9250() {
    if (file_ >= 0) close(file_);
}

bool MPU9250::initialize() {
    file_ = open(device_, O_RDWR);
    if (file_ < 0 || ioctl(file_, I2C_SLAVE, address_) < 0) {
        std::cerr << "MPU9250 nicht erreichbar\n";
        return false;
    }
    return i2c_smbus_write_byte_data(file_, PWR_MGMT_1, 0x00) == 0;
}

int16_t MPU9250::readWord(uint8_t reg) {
    int high = i2c_smbus_read_byte_data(file_, reg);
    int low  = i2c_smbus_read_byte_data(file_, reg + 1);
    int16_t val = (high << 8) | low;
    return (val >= 0x8000) ? -((65535 - val) + 1) : val;
}

int16_t MPU9250::readMagWord(uint8_t reg) {
    int low = i2c_smbus_read_byte_data(file_, reg);
    int high  = i2c_smbus_read_byte_data(file_, reg + 1);
    return (high << 8) | low;
}


bool MPU9250::readAccel(float& ax, float& ay, float& az) {
    ax = readWord(ACCEL_XOUT_H)     / ACCEL_SCALE * GRAVITY_MS2; // Accel [m/s²]
    ay = readWord(ACCEL_XOUT_H + 2) / ACCEL_SCALE * GRAVITY_MS2; // Accel [m/s²]
    az = readWord(ACCEL_XOUT_H + 4) / ACCEL_SCALE * GRAVITY_MS2; // Accel [m/s²]
    return true;
}

bool MPU9250::readGyro(float& gx, float& gy, float& gz) {
    gx = readWord(GYRO_XOUT_H)     / GYRO_SCALE; // Gyro  [°/s]
    gy = readWord(GYRO_XOUT_H + 2) / GYRO_SCALE; // Gyro  [°/s]
    gz = readWord(GYRO_XOUT_H + 4) / GYRO_SCALE; // Gyro  [°/s]
    return true;
}

bool MPU9250::readMag(float& mag_x, float& mag_y, float& mag_z){
    mag_x = readMagWord(HXL) / MAGNETO_SCALE; // Magn  [µT] Mikrotesla
    mag_y = readMagWord(HYL) / MAGNETO_SCALE; // Magn  [µT]
    mag_z = readMagWord(HZL) / MAGNETO_SCALE; // Magn  [µT]
    return true;
}

void MPU9250::visSensDat(const std::array<float, 3>& accel, 
                        const std::array<float, 3>& accel_fil, 
                        const std::array<float, 3>& gyr, 
                        const std::array<float, 3>& gyr_fil,
                        const std::uint32_t counter){

    static bool plotting_initialized = false;
    if (!plotting_initialized) {
        plt::ion();
        plotting_initialized = true;
    }
    
    if (ax_vals.size() >= MAX_POINTS) {
        time_vals.pop_front();
        ax_vals.pop_front();    ay_vals.pop_front();    az_vals.pop_front();
        ax_valsFil.pop_front(); ay_valsFil.pop_front(); az_valsFil.pop_front();
        gx_vals.pop_front();    gy_vals.pop_front();    gz_vals.pop_front();
        gx_valsFil.pop_front(); gy_valsFil.pop_front(); gz_valsFil.pop_front();
    }

    time_vals.push_back(counter * 0.2f);
    ax_vals.push_back(accel[0]);      ay_vals.push_back(accel[1]);      az_vals.push_back(accel[2]);
    ax_valsFil.push_back(accel_fil[0]); ay_valsFil.push_back(accel_fil[1]); az_valsFil.push_back(accel_fil[2]);
    gx_vals.push_back(gyr[0]);        gy_vals.push_back(gyr[1]);        gz_vals.push_back(gyr[2]);
    gx_valsFil.push_back(gyr_fil[0]); gy_valsFil.push_back(gyr_fil[1]); gz_valsFil.push_back(gyr_fil[2]);
    
    // Plot aktualisieren
    plt::clf();
    plt::subplot(2, 2, 1);
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(ax_vals.begin(),   ax_vals.end()),
              "r-");
    plt::plot(std::vector<float>(time_vals.begin(),  time_vals.end()),
              std::vector<float>(ax_valsFil.begin(), ax_valsFil.end()),
              "b-");                      
    plt::title("Long Acceleration [m/s²]");
    plt::xlabel("Time [s]");
    plt::ylabel("a_x");
    plt::grid(true);

    // Lateral acceleration
    plt::subplot(2, 2, 2);
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(ay_vals.begin(), ay_vals.end()),
              "r-");
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(ay_valsFil.begin(), ay_valsFil.end()),
              "b-");                      
    plt::title("Lateral Acceleration [m/s²]");
    plt::xlabel("Time [s]");
    plt::ylabel("a_y");
    plt::grid(true);       
    
    plt::subplot(2, 2, 3);
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(gx_vals.begin(), gx_vals.end()),
              "r-");
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(gx_valsFil.begin(), gx_valsFil.end()),
              "b-");                      
    plt::title("Pitch Rate [°/s]");
    plt::xlabel("Time [s]");
    plt::ylabel("g_x");
    plt::grid(true);
    // Ploting of 
    plt::subplot(2, 2, 4);
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(gy_vals.begin(), gy_vals.end()),
              "r-");
    plt::plot(std::vector<float>(time_vals.begin(), time_vals.end()),
              std::vector<float>(gy_valsFil.begin(), gy_valsFil.end()),
              "b-");                      
    plt::title("Roll Rate [°/s]");
    plt::xlabel("Time [s]");
    plt::ylabel("g_y");
    plt::grid(true); 
    
    plt::pause(0.0001);  // kurz pausieren für Update

}