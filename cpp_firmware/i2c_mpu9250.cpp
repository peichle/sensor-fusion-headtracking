#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cmath>

#define MPU_ADDR 0x68
#define MPU_REG_PWR_MGMT_1 0x6B
#define MPU_REG_ACCEL_XOUT_H 0x3B

int16_t read_word_2c(int file, int addr) {
    uint8_t high = i2c_smbus_read_byte_data(file, addr);
    uint8_t low  = i2c_smbus_read_byte_data(file, addr + 1);
    int16_t val = (high << 8) | low;
    if (val >= 0x8000) val = -((65535 - val) + 1);
    return val;
}

int main() {
    const char* device = "/dev/i2c-1";
    int file = open(device, O_RDWR);
    if (file < 0) {
        std::cerr << "Failed to open I2C device.\n";
        return 1;
    }

    if (ioctl(file, I2C_SLAVE, MPU_ADDR) < 0) {
        std::cerr << "Failed to connect to the MPU9250.\n";
        return 1;
    }

    // Wake up the MPU
    i2c_smbus_write_byte_data(file, MPU_REG_PWR_MGMT_1, 0x00);
    usleep(100000);

    while (true) {
        int16_t accel_x = read_word_2c(file, MPU_REG_ACCEL_XOUT_H);
        int16_t accel_y = read_word_2c(file, MPU_REG_ACCEL_XOUT_H + 2);
        int16_t accel_z = read_word_2c(file, MPU_REG_ACCEL_XOUT_H + 4);

        int16_t gyro_x = read_word_2c(file, 0x43);
        int16_t gyro_y = read_word_2c(file, 0x45);
        int16_t gyro_z = read_word_2c(file, 0x47);

        std::cout << "Accel [g]: X=" << accel_x / 16384.0
                  << " Y=" << accel_y / 16384.0
                  << " Z=" << accel_z / 16384.0 << "\n";

        std::cout << "Gyro [°/s]: X=" << gyro_x / 131.0
                  << " Y=" << gyro_y / 131.0
                  << " Z=" << gyro_z / 131.0 << "\n\n";

        usleep(200000); // 5 Hz
    }

    close(file);
    return 0;
}
