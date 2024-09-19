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


std::vector<triangle> Shape::generate_mesh(double x, double y, double z) {

    std::vector<triangle> mesh;

    // Init points of cube
    cv::Mat Cube_cubeP0 = createPoint(-x,  y, -z);
    cv::Mat Cube_cubeP1 = createPoint(-x, -y, -z);
    cv::Mat Cube_cubeP2 = createPoint( x, -y, -z);
    cv::Mat Cube_cubeP3 = createPoint( x,  y, -z);
    cv::Mat Cube_cubeP4 = createPoint(-x,  y,  z);
    cv::Mat Cube_cubeP5 = createPoint(-x, -y,  z);
    cv::Mat Cube_cubeP6 = createPoint( x, -y,  z);
    cv::Mat Cube_cubeP7 = createPoint( x,  y,  z);

    mesh = {
        // Top face
        { {Cube_cubeP4, Cube_cubeP5, Cube_cubeP6} },
        { {Cube_cubeP4, Cube_cubeP6, Cube_cubeP7} },

        // Bottom face
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


void Shape::set_position(double x, double y, double z, triangle* tri) {

    double temp_matrix[4*4] = {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    cv::Mat translation_matrix(4, 4, CV_64F, temp_matrix);


    for (int i = 0; i < 3; ++i) {

        cv::Mat point = tri->point[i];
        cv::Mat transformed_point = (translation_matrix) * point;

        tri->point[i] = transformed_point;

        std::cout << tri->point[i] << std::endl;
    }
    
}