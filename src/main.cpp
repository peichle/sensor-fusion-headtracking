#include "../include/i2c_mpu9250.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

bool create_log{true};

int main() {
    MPU9250 imu;
    if (!imu.initialize()) return 1;

    std::ofstream logfile("data/mpu9250_log.txt");

    while (true) {
        float ax, ay, az, gx, gy, gz, mag_x, mag_y, mag_z;
        imu.readAccel(ax, ay, az);
        imu.readGyro(gx, gy, gz);
        imu.readMag(mag_x, mag_y, mag_z);

        std::cout << "Accel [m/s²]: X=" << ax << " Y=" << ay << " Z=" << az << "\n";
        std::cout << "Gyro  [°/s] : X=" << gx << " Y=" << gy << " Z=" << gz << "\n";
        std::cout << "Magn  [µT]  : X=" << mag_x << " Y=" << mag_y << " Z=" << mag_z << "\n\n";

        if (create_log == true)
        {
            logfile << ax << " " << ay << " " << az << " " 
                    << gx << " " << gy << " " << gz << std::endl;
            logfile.flush();
        }
        usleep(200000); // 5 Hz
    }
}
