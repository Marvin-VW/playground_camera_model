#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class CubeTracker {
public:
    CubeTracker(int frame_width, int frame_height);
    
    std::vector<cv::Point2d> trackCube(const cv::Mat& frame);

private:
    cv::Scalar lower_pink;
    cv::Scalar upper_pink;
    int frame_width;
    int frame_height;
    cv::Mat hls_frame, mask;
};
