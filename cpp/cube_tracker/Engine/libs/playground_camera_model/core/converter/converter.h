#pragma once 

#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>
#include "Shape.h"

class Converter {
public:
    Converter(cv::Mat& I_T_C);
    void setBasePoints(const std::vector<cv::Point2d>& base_points, double side_length);
    void computeCubePoints();
    std::vector<cv::Mat> getCubePoints() const;
    std::vector<triangle> computeMesh();

private:
    cv::Mat I_T_C;
    double side_length;
    std::vector<cv::Point2d> base_points;
    std::vector<cv::Mat> cube_points;
    cv::Mat createPoint(double x, double y, double z);
};

