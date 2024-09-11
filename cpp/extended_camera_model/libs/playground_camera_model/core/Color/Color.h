// Copyright (C) 2024 Marvin-VW
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include <cmath>
#include <opencv2/opencv.hpp>

class Color {
public:

    double intensity(cv::Vec3f light_direction, cv::Mat& normal);
    cv::Scalar bgr_to_hsl(const cv::Scalar& bgr);
    cv::Scalar hsl_to_bgr(const cv::Scalar& hls);
    cv::Scalar adjust_bgr_intensity(cv::Scalar& base_color, double intensity) ;

};
