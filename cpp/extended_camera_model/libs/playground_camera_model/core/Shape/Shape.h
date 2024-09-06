#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;

struct triangle
{
	cv::Mat point[3];
};

struct mesh
{
	vector<triangle> tris;
};

class Shape {

public:

	cv::Mat createPoint(double x, double y, double z);
    std::vector<triangle> generate_mesh();



    // Set position of the shape
    void setPosition(float x, float y, float z);

    // Set scale of the shape
    void setScale(float scaleX, float scaleY, float scaleZ);

    // Set rotation of the shape
    void setRotation(float rotX, float rotY, float rotZ);

};