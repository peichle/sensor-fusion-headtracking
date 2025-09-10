#pragma once

#include <cstdint>
#include <array>
#include <deque>


class MPU9250 {
public:
    MPU9250(const char* device = "/dev/i2c-1", int address = 0x68);
    ~MPU9250();

    bool initialize();
    bool readAccel(float& ax, float& ay, float& az);
    bool readGyro(float& gx, float& gy, float& gz);
    bool readMag(float& mag_x, float& mag_y, float& mag_z);
    void visSensDat(const std::array<float, 3>& accel, const std::array<float, 3>& accel_fil, 
        const std::array<float, 3>& gyr, const std::array<float, 3>& gyr_fil,
        const std::uint32_t counter);

private:
    int file_;
    const char* device_;
    int address_;
    inline static std::deque<float> time_vals,
                            ax_vals, ay_vals, az_vals,
                            ax_valsFil, ay_valsFil, az_valsFil,
                            gx_vals, gy_vals, gz_vals,
                            gx_valsFil, gy_valsFil, gz_valsFil;

    const int MAX_POINTS = 100;
    

    int16_t readWord(uint8_t reg);
    int16_t readMagWord(uint8_t reg);

};
