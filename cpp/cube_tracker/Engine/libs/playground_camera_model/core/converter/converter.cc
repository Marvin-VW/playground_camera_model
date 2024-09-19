#include "converter.h"

Converter::Converter(cv::Mat& I_T_C) : I_T_C(I_T_C) {
}

cv::Mat Converter::createPoint(double x, double y, double z){

	cv::Mat point(4,1,CV_64F);

	point.at<double>(0) = x;
	point.at<double>(1) = y;
	point.at<double>(2) = z;
	point.at<double>(3) = 1;

	return point;
}

void Converter::setBasePoints(const std::vector<cv::Point2d>& base_points, double side_length) {
    if (base_points.size() != 4) {
        throw std::invalid_argument("Exactly 4 base points are required.");
    }
    this->base_points = base_points;
    this->side_length = side_length;
}

void Converter::computeCubePoints() {

    cube_points.clear();

    cv::Mat I_T_C_4x4 = cv::Mat::zeros(4, 4, CV_64F);

    I_T_C.copyTo(I_T_C_4x4(cv::Rect(0, 0, 4, 3)));

    I_T_C_4x4.at<double>(3, 0) = 0;
    I_T_C_4x4.at<double>(3, 1) = 0;
    I_T_C_4x4.at<double>(3, 2) = 0;
    I_T_C_4x4.at<double>(3, 3) = 1;


    std::cout << I_T_C_4x4 << std::endl;



    for (const auto& base_point : base_points) {


        cv::Mat base_camera_point = createPoint(base_point.x, base_point.y, 1.0);
        cv::Mat C_point_base = I_T_C_4x4.inv() * base_camera_point;
        cube_points.push_back(C_point_base);

    }

    side_length = abs(cube_points[0].at<double>(0) - cube_points[1].at<double>(0));

    std::cout << side_length << std::endl;

    for (const auto& base_point : base_points) {

        //(z = side_length)
        cv::Mat top_camera_point = createPoint(base_point.x, base_point.y, 1.0);
        cv::Mat C_point_top = I_T_C_4x4.inv() * top_camera_point;
        C_point_top.at<double>(2) =  C_point_top.at<double>(2) + side_length; //modify z by simulated depth
        cube_points.push_back(C_point_top);
    }
}


std::vector<triangle> Converter::computeMesh() {

    std::vector<triangle> mesh;

    mesh = {
        // Top face
        { {cube_points[0], cube_points[4], cube_points[5]} },
        { {cube_points[0], cube_points[1], cube_points[5]} },

        // Bottom face
        { {cube_points[2], cube_points[6], cube_points[7]} },
        { {cube_points[3], cube_points[2], cube_points[7]} },

        // Left face
        { {cube_points[2], cube_points[6], cube_points[4]} },
        { {cube_points[0], cube_points[2], cube_points[4]} },

        // Right face
        { {cube_points[3], cube_points[7], cube_points[5]} },
        { {cube_points[1], cube_points[3], cube_points[5]} },

        // Top face
        { {cube_points[0], cube_points[4], cube_points[5]} },
        { {cube_points[0], cube_points[1], cube_points[5]} },

        // Bottom face
        { {cube_points[2], cube_points[6], cube_points[7]} },
        { {cube_points[3], cube_points[2], cube_points[7]} }
    };

    return mesh;

}

std::vector<cv::Mat> Converter::getCubePoints() const {
    return cube_points;
}
