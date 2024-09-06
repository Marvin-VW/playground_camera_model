// Copyright (C) 2024 Marvin-VW
#include <iostream>
#include <vector>
#include "Shape.h"

cv::Mat Shape::createPoint(double x, double y, double z){

	cv::Mat point(4,1,CV_64F);

	point.at<double>(0) = x;
	point.at<double>(1) = y;
	point.at<double>(2) = z;
	point.at<double>(3) = 1;

	return point;
}


std::vector<triangle> Shape::generate_mesh() {

    std::vector<triangle> mesh;

    // Init points of cube
    cv::Mat Cube_cubeP0 = createPoint(-1,  1, -1);
    cv::Mat Cube_cubeP1 = createPoint(-1, -1, -1);
    cv::Mat Cube_cubeP2 = createPoint( 1, -1, -1);
    cv::Mat Cube_cubeP3 = createPoint( 1,  1, -1);
    cv::Mat Cube_cubeP4 = createPoint(-1,  1,  1);
    cv::Mat Cube_cubeP5 = createPoint(-1, -1,  1);
    cv::Mat Cube_cubeP6 = createPoint( 1, -1,  1);
    cv::Mat Cube_cubeP7 = createPoint( 1,  1,  1);

    mesh = {
        // Front face
        { {Cube_cubeP4, Cube_cubeP5, Cube_cubeP6} },
        { {Cube_cubeP4, Cube_cubeP6, Cube_cubeP7} },

        // Back face
        { {Cube_cubeP1, Cube_cubeP0, Cube_cubeP2} },
        { {Cube_cubeP2, Cube_cubeP0, Cube_cubeP3} },

        // Left face
        { {Cube_cubeP3, Cube_cubeP0, Cube_cubeP7} },
        { {Cube_cubeP7, Cube_cubeP0, Cube_cubeP4} },

        // Right face
        { {Cube_cubeP5, Cube_cubeP1, Cube_cubeP6} },
        { {Cube_cubeP6, Cube_cubeP1, Cube_cubeP2} },

        // Top face
        { {Cube_cubeP4, Cube_cubeP0, Cube_cubeP5} },
        { {Cube_cubeP5, Cube_cubeP0, Cube_cubeP1} },

        // Bottom face
        { {Cube_cubeP2, Cube_cubeP3, Cube_cubeP6} },
        { {Cube_cubeP6, Cube_cubeP3, Cube_cubeP7} }
    };

    return mesh;
}


void Shape::setPosition(float x, float y, float z) {
    std::cout << "Setting position to (" << x << ", " << y << ", " << z << ")" << std::endl;
}

void Shape::setScale(float scaleX, float scaleY, float scaleZ) {
    std::cout << "Setting scale to (" << scaleX << ", " << scaleY << ", " << scaleZ << ")" << std::endl;
}

void Shape::setRotation(float rotX, float rotY, float rotZ) {
    std::cout << "Setting rotation to (" << rotX << ", " << rotY << ", " << rotZ << ")" << std::endl;
}