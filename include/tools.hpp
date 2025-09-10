#pragma once

#include <vector>
#include <array>

std::vector<int> euler_winkel();


void rotationMatrixToEuler(const float R[3][3], float eulerAngles[3]);


class LowPassFilter {
private:
 //   float alpha;           // smoothing factor (0 < alpha < 1)
//    bool is_initialized_;  // for first update
//    float prev_output_[3]; // previous filtered value
    std::array<float, 3> prev_output_; // previous filtered value

public:
 //   void LowPassFilter(float alpha);
    void reset();
    void update(const std::array<float, 3>& input, std::array<float, 3>& output, const float alpha); // input, output, smoothing factor
};
