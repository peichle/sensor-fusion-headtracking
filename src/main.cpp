#include "../include/i2c_mpu9250.hpp"
#include "../include/tools.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

bool create_log{false};
bool create_plot{true};



int main() {
    MPU9250 imu;
    if (!imu.initialize()) return 1;

    std::ofstream logfile("data/mpu9250_log.txt");


    std::uint32_t counter = 0;

    while (true) {
        float ax, ay, az, gx, gy, gz, mag_x, mag_y, mag_z;
        imu.readAccel(ax, ay, az);
        imu.readGyro(gx, gy, gz);
        imu.readMag(mag_x, mag_y, mag_z);

        LowPassFilter lpfilter;

        std::array<float, 3> accel = {ax, ay, az}; 
        std::array<float, 3> accel_fil{};
        lpfilter.update(accel, accel_fil, 0.9);

        std::array<float, 3> gyr = {gx, gy, gz};
        std::array<float, 3> gyr_fil{};
        lpfilter.update(gyr, gyr_fil, 0.6);

/*
        std::cout << "Accel [m/s²]: X=" << ax << " Y=" << ay << " Z=" << az << "\n";
        std::cout << "Gyro  [°/s] : X=" << gx << " Y=" << gy << " Z=" << gz << "\n";
        std::cout << "Magn  [µT]  : X=" << mag_x << " Y=" << mag_y << " Z=" << mag_z << "\n\n";
*/
        if (create_plot) {
            imu.visSensDat(accel, accel_fil, gyr, gyr_fil, counter);
        }

        if (create_log) {
            logfile << ax << " " << ay << " " << az << " " 
                    << gx << " " << gy << " " << gz << "\n";
            logfile.flush();
        }

        ++counter;
        usleep(200000); // 5 Hz
    }
}
