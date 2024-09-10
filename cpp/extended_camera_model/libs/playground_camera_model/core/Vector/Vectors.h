// Copyright (C) 2024 Marvin-VW

#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <set>
#include "Shape.h"


class Vectors {

public:

    cv::Mat vector(const cv::Mat& point1, const cv::Mat& point2);
    std::tuple<cv::Mat, cv::Mat> normal(triangle& tri, float scale = 0.5f);
    //std::vector<triangle> get_shadow(const std::vector<triangle>& triangles, const cv::Vec3f& light_vec);
    //cv::Vec3f find_intersection(const cv::Vec3f& plane_normal, const cv::Vec3f& line_point, const cv::Vec3f& line_dir, float plane_d = 2.0f);


};