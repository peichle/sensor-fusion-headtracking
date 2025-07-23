#ifndef I2C_MPU9250_HPP
#define I2C_MPU9250_HPP

#include <cstdint>

class MPU9250 {
public:
    MPU9250(const char* device = "/dev/i2c-1", int address = 0x68);
    ~MPU9250();

    bool initialize();
    bool readAccel(float& ax, float& ay, float& az);
    bool readGyro(float& gx, float& gy, float& gz);
    bool readMag(float& mag_x, float& mag_y, float& mag_z);

private:
    int file_;
    const char* device_;
    int address_;

    int16_t readWord(uint8_t reg);
    int16_t readMagWord(uint8_t reg);
};

#endif // I2C_MPU9250_HPP
