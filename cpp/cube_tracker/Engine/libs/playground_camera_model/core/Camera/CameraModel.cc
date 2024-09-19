// Copyright (C) 2024 twyleg, Marvin-VW
#include "CameraModel.h"
#include "Shape.h"
#include <opencv2/opencv.hpp>

CameraModel::CameraModel(double sensorWidth, double sensorHeight, double focalLength, uint32_t resolutionX, uint32_t resolutionY, uint32_t u0, uint32_t v0) {
    this->sensorWidth = sensorWidth;
    this->sensorHeight = sensorHeight;
    this->focalLength = focalLength;
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
    this->u0 = u0;
    this->v0 = v0;

    cameraImage.create(resolutionY, resolutionX, CV_8UC3);
    cameraImage = cv::Scalar(255, 255, 255);

    const double rhoWidth = sensorWidth / resolutionX;
    const double rhoHeight = sensorHeight / resolutionY;

    double matrixK_data[3 * 3] = {
        (1 / rhoWidth), 0, static_cast<double>(u0),
        0, (1 / rhoHeight), static_cast<double>(v0),
        0, 0, 1
    };

    double matrixC_data[3 * 4] = {
        focalLength, 0, 0, 0,
        0, focalLength, 0, 0,
        0, 0, 1, 0
    };

    const cv::Mat matrixK(3, 3, CV_64F, matrixK_data);
    const cv::Mat matrixC(3, 4, CV_64F, matrixC_data);

    I_T_C.create(3, 4, CV_64F);
    I_T_C = matrixK * matrixC;
}

void CameraModel::drawAllPoints(const std::vector<triangle>* mesh) {
    for (const auto& tri : *mesh) {
        for (const auto& vertex : tri.camera_points) {
            drawCameraImagePoint(vertex);
        }
    }
}

void CameraModel::drawCameraImagePoint(const cv::Mat& C_point){

	const cv::Mat I_point = I_T_C * C_point;

	const int32_t u = I_point.at<double>(0) / I_point.at<double>(2);
	const int32_t v = I_point.at<double>(1) / I_point.at<double>(2);

	const cv::Point point(u,v);

    std::cout << "Pint" << point << std::endl;

	cv::circle(cameraImage, point, 5, cv::Scalar(255,0,0), 2);

}

void CameraModel::drawAllLines(const std::vector<triangle>* mesh) {
    for (const auto& tri : *mesh) {
        drawCameraImageLine(tri.camera_points[0], tri.camera_points[1]);
        drawCameraImageLine(tri.camera_points[1], tri.camera_points[2]);
        drawCameraImageLine(tri.camera_points[2], tri.camera_points[0]);
    }
}


void CameraModel::drawCameraImageLine(const cv::Mat& C_point0, const cv::Mat& C_point1){

	const cv::Mat I_point0 = I_T_C * C_point0;
	const cv::Mat I_point1 = I_T_C * C_point1;

	const int32_t u0 = I_point0.at<double>(0) / I_point0.at<double>(2);
	const int32_t v0 = I_point0.at<double>(1) / I_point0.at<double>(2);

	const int32_t u1 = I_point1.at<double>(0) / I_point1.at<double>(2);
	const int32_t v1 = I_point1.at<double>(1) / I_point1.at<double>(2);

	const cv::Point point0(u0,v0);
	const cv::Point point1(u1,v1);

	cv::line(cameraImage, point0, point1, cv::Scalar(255,0,0), 1);
}

void CameraModel::drawCameraImageArrow(const cv::Mat& C_point0, const cv::Mat& C_point1) {
    try {
        // Compute image points
        cv::Mat I_point0 = I_T_C * C_point0;
        cv::Mat I_point1 = I_T_C * C_point1;

        // Convert to homogeneous coordinates
        double u0 = I_point0.at<double>(0) / I_point0.at<double>(2);
        double v0 = I_point0.at<double>(1) / I_point0.at<double>(2);
        double u1 = I_point1.at<double>(0) / I_point1.at<double>(2);
        double v1 = I_point1.at<double>(1) / I_point1.at<double>(2);

        // Draw the arrowed line
        cv::arrowedLine(cameraImage, cv::Point(static_cast<int>(u0), static_cast<int>(v0)),
                        cv::Point(static_cast<int>(u1), static_cast<int>(v1)),
                        cv::Scalar(0, 255, 0), 2);
    } catch (const std::exception& e) {
        throw std::runtime_error("Could not draw normal: " + std::string(e.what()));
    }
}


void CameraModel::fillCubeFaces(const std::vector<triangle>* mesh) {
    for (const auto& tri : *mesh) {
        std::vector<cv::Point> I_points;

        for (const auto& C_point : tri.camera_points) {
            cv::Mat I_point = I_T_C * C_point;

            int u = static_cast<int>(I_point.at<double>(0) / I_point.at<double>(2));
            int v = static_cast<int>(I_point.at<double>(1) / I_point.at<double>(2));

            I_points.emplace_back(u, v);
        }

        cv::Mat Poly_Points(I_points);
        cv::fillPoly(cameraImage, Poly_Points, tri.color);
    }
}

void CameraModel::world_transform(const cv::Mat* matrix, triangle* tri) {
        
    for (int i = 0; i < 3; ++i) {

        cv::Mat point = tri->point[i];
        cv::Mat transformed_point = (*matrix) * point;

        tri->world_points[i] = transformed_point;
    }
    
}

void CameraModel::camera_transform(const cv::Mat* matrix, triangle* tri) {
    for (int i = 0; i < 3; ++i) {
        cv::Mat point = tri->camera_points[i];
        cv::Mat transformed_point = (*matrix).inv() * point;

        tri->world_points[i] = transformed_point;
    }
}

cv::Vec3f CameraModel::getCameraVector(const cv::Mat& V_T_C) {
    cv::Mat rotation_matrix = V_T_C(cv::Rect(0, 0, 3, 3));

    cv::Vec3f forward_vector(-rotation_matrix.at<double>(0, 2),
                             -rotation_matrix.at<double>(1, 2),
                             -rotation_matrix.at<double>(2, 2));

    cv::Vec3f camera_position(V_T_C.at<double>(0, 3),
                              V_T_C.at<double>(1, 3),
                              V_T_C.at<double>(2, 3));

    double final_vector_x = forward_vector[0] + camera_position[0];
    double final_vector_y = forward_vector[1] + camera_position[1];
    double final_vector_z = forward_vector[2] + camera_position[2];

    return cv::Vec3f(final_vector_x, final_vector_y, final_vector_z);
}

void CameraModel::resetCameraImage(cv::Mat frame) {
    cameraImage = frame;
}

cv::Mat& CameraModel::getCameraImage() {
    return cameraImage;
}