// Copyright (C) 2024 Marvin-VW
#include "Vectors.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <set>
#include "Shape.h"

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

const cv::Mat appendHomogeneousCoordinate(const cv::Mat& point);

cv::Mat Vectors::vector(const cv::Mat& point1, const cv::Mat& point2) {
    return point2 - point1;
}

std::tuple<cv::Mat, cv::Mat> Vectors::normal(triangle& tri, float scale) {

    cv::Mat p1 = tri.world_points[0].rowRange(0, 3);
    cv::Mat p2 = tri.world_points[1].rowRange(0, 3);
    cv::Mat p3 = tri.world_points[2].rowRange(0, 3);

    // vectors
    cv::Mat vec1 = vector(p1, p2);
    cv::Mat vec2 = vector(p1, p3);

    // normal vector
    cv::Mat normal_vector = vec1.cross(vec2);

    // normalize to unit length
    float norm = cv::norm(normal_vector);
    if (norm == 0) norm = 0.5f;
    normal_vector /= norm;

    cv::Mat centroid = (p1 + p2 + p3) / 3.0;

    tri.centroid = centroid;

    // scale vector
    cv::Mat scaled_normal = normal_vector * scale;

    tri.normal = scaled_normal;

    // start and end of normal vector
    cv::Mat normal_start = cv::Mat(centroid);
    cv::Mat normal_end = cv::Mat(centroid + scaled_normal);

    return {appendHomogeneousCoordinate(normal_start), appendHomogeneousCoordinate(normal_end)};
}


const cv::Mat appendHomogeneousCoordinate(const cv::Mat& point)
{
    cv::Mat homogenous_point(4, 1, CV_64F);

    homogenous_point.at<double>(0) = point.at<double>(0);
    homogenous_point.at<double>(1) = point.at<double>(1);
    homogenous_point.at<double>(2) = point.at<double>(2);
    
    homogenous_point.at<double>(3) = 1.0;

    return homogenous_point;
}