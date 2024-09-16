// Copyright (C) 2024 Marvin-VW
#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;

struct triangle
{
	cv::Mat point[3];
    cv::Mat camera_points[3];
    cv::Mat world_points[3];
    cv::Mat normal;
    cv::Mat centroid;
    double ilm;
    cv::Scalar color;
};

struct mesh
{
	vector<triangle> tris;
};

class Shape {

public:

	cv::Mat createPoint(double x, double y, double z);
    std::vector<triangle> generate_mesh();
    void set_position(float x, float y, float z, std::vector<triangle>* mesh);

};