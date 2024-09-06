// Copyright (C) 2024 twyleg
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

void CameraModel::drawAllPoints(const std::vector<triangle>& mesh) {
    for (const auto& tri : mesh) {
        for (const auto& vertex : tri.point) {
            drawCameraImagePoint(vertex);
        }
    }
}

void CameraModel::drawCameraImagePoint(const cv::Mat& C_point){

	const cv::Mat I_point = I_T_C * C_point;

	const int32_t u = I_point.at<double>(0) / I_point.at<double>(2);
	const int32_t v = I_point.at<double>(1) / I_point.at<double>(2);

	const cv::Point point(u,v);

	cv::circle(cameraImage, point, 5, cv::Scalar(255,0,0), 2);

}

void CameraModel::drawAllLines(const std::vector<triangle>& mesh) {
    for (const auto& tri : mesh) {
        drawCameraImageLine(tri.point[0], tri.point[1]);
        drawCameraImageLine(tri.point[1], tri.point[2]);
        drawCameraImageLine(tri.point[2], tri.point[0]);
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

void CameraModel::transform(const cv::Mat* matrix, const std::vector<triangle>& original_mesh, std::vector<triangle>& new_mesh) {
    new_mesh.clear();
    
    // Iterate over each triangle in the original mesh
    for (const auto& tri : original_mesh) {
        triangle new_tri;
        
        // Iterate over each vertex in the triangle
        for (int i = 0; i < 3; ++i) {
            // Apply the transformation matrix to the vertex
            cv::Mat point = tri.point[i];
            cv::Mat transformed_point = (*matrix) * point;

            // Update the new triangle with the transformed coordinates
            new_tri.point[i] = transformed_point;
        }
        
        // Add the transformed triangle to the new mesh
        new_mesh.push_back(new_tri);
    }
}



void CameraModel::resetCameraImage() {
    cameraImage = cv::Scalar(255, 255, 255);
}

cv::Mat& CameraModel::getCameraImage() {
    return cameraImage;
}