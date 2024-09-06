// Copyright (C) 2024 twyleg, Marvin-VW
#pragma once

#include <cstdint>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Shape.h"

class CameraModel {

private:
    double sensorWidth;
    double sensorHeight;
    double focalLength;
    uint32_t resolutionX;
    uint32_t resolutionY;
    uint32_t u0;
    uint32_t v0;

    cv::Mat cameraImage;

public:
    cv::Mat I_T_C;
    cv::Mat V_T_C;
    cv::Mat C_T_V;
    cv::Mat V_T_Cube;

    CameraModel(double sensorWidth,
                double sensorHeight,
                double focalLength,
                uint32_t resolutionX,
                uint32_t resolutionY,
                uint32_t u0,
                uint32_t v0);

    void drawCameraImagePoint(const cv::Mat& point);
    void drawAllPoints(const std::vector<triangle>& mesh);
    void drawCameraImageLine(const cv::Mat& point0, const cv::Mat& point1);
    void drawAllLines(const std::vector<triangle>& mesh);
    void transform(const cv::Mat* matrix, const std::vector<triangle>& original_mesh, std::vector<triangle>& new_mesh);

    void resetCameraImage();
    cv::Mat& getCameraImage();
};