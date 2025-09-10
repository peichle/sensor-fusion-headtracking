
#include <../include/tools.hpp>
#include <cmath>




std::vector<int> euler_winkel(int32_t q0, int32_t q1, int32_t q2, int32_t q3)
{
    std::vector<int> euler_value(3);
    euler_value[0] = atan2(2*(q0*q1 + q2*q3),  1 - 2*(q1*q1 + q2*q2)); // pitch
    euler_value[1] = asin(2*(q0*q2 - q3*q1)); // roll
    euler_value[3] = atan2(2*(q0*q3 + q1*q2),  1 - 2*(q2*q2 + q3*q3)); // yaw

    return euler_value;
}

void rotationMatrixToEuler(const float R[3][3], float eulerAngles[3])
{
    // assumption: ZYX --> Yaw-Pitch-Roll
    float sy = std::sqrt(R[0][0] * R[0][0] + R[1][0]*R[1][0] );

    bool singular = sy < 1e-6;

    if (!singular)
    {
        eulerAngles[0] = std::atan2(R[2][1], R[2][2]); // Roll
        eulerAngles[1] = std::atan2(-R[2][0], sy);      // Pitch 
        eulerAngles[2] = std::atan2(R[1][0], R[0][0]); //  Yaw
    }
    else
    {
        eulerAngles[0] = std::atan2(-R[1][2], R[1][1]); // Roll
        eulerAngles[1] = std::atan2(-R[2][0], sy);      // Pitch 
        eulerAngles[2] = 0; //  Yaw
    }
}



//LowPassFilter::LowPassFilter(float alpha) 
//: alpha_(alpha), is_initialized_(false), prev_output_{0.0f, 0.0f, 0.0f} {}
//void LowPassFilter::reset(){
//    is_initialized_ = false;
//}

//void LowPassFilter::update(const float input[3], float output[3]) {
//    if(!is_initialized_){
//        for (size_t i = 0; i < 3; i++)
//        {
//            prev_output_[i] = input[i];
//        }
//        is_initialized_ = true;            
//    }
//    for (size_t i = 0; i < 3; i++)
//    {
//        prev_output_[i] = alpha_ * input[i] + (1.0f - alpha_) * prev_output_[i];
//        output[i] = prev_output_[i];
//    }
//    
//}


void LowPassFilter::update(const std::array<float, 3>& input, std::array<float, 3>& output, const float alpha) {
//    if (!is_initialized_) {
//        prev_output_ = input;
//        is_initialized_ = true;
//    }

    for (size_t i = 0; i < 3; ++i) {
        prev_output_[i] = alpha * input[i] + (1.0f - alpha) * prev_output_[i];
        output[i] = prev_output_[i];
    }
}
