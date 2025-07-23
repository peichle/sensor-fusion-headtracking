#include "../include/i2c_mpu9250.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <iostream>

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