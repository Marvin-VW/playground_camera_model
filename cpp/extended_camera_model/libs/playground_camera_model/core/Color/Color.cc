// Copyright (C) 2024 Marvin-VW
#include "Color.h"
#include <opencv2/opencv.hpp>
    

double Color::intensity(cv::Vec3f light_direction, cv::Mat& normal) 
{
    cv::Vec3f normalized_light_direction;
    cv::normalize(light_direction, normalized_light_direction);
    double intensity = normalized_light_direction.dot(normal) * (-1.0);
    return intensity;
}

cv::Scalar Color::bgr_to_hsl(const cv::Scalar& bgr) 
{
    cv::Mat bgrMat(1, 1, CV_8UC3, bgr);
    cv::Mat hlsMat;
    cv::cvtColor(bgrMat, hlsMat, cv::COLOR_BGR2HLS);
    cv::Scalar hls = hlsMat.at<cv::Vec3b>(0, 0);
    return hls;
}

cv::Scalar Color::hsl_to_bgr(const cv::Scalar& hls) 
{
    cv::Mat hlsMat(1, 1, CV_8UC3, hls);
    cv::Mat bgrMat;
    cv::cvtColor(hlsMat, bgrMat, cv::COLOR_HLS2BGR);
    cv::Scalar bgr = bgrMat.at<cv::Vec3b>(0, 0);
    return bgr;
}

cv::Scalar Color::adjust_bgr_intensity(cv::Scalar& base_color, double intensity) 
{
    cv::Scalar hsl = bgr_to_hsl(base_color);
    hsl[1] = cv::saturate_cast<uchar>(hsl[1] * intensity);
    return hsl_to_bgr(hsl);
}
