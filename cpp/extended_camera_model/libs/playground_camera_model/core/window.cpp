// Copyright (C) 2024 Daniel-VW
#include "window.h"

void Window::createCameraSettingsWindow(int32_t* cameraSystemTranslationX, int32_t* cameraSystemTranslationY, int32_t* cameraSystemTranslationZ,
                                           int32_t* cameraSystemRotationRoll, int32_t* cameraSystemRotationPitch, int32_t* cameraSystemRotationYaw) {
    
    cv::namedWindow("camera settings", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("X", "camera settings", cameraSystemTranslationX, 20000);
    cv::createTrackbar("Y", "camera settings", cameraSystemTranslationY, 20000);
    cv::createTrackbar("Z", "camera settings", cameraSystemTranslationZ, 20000);
    cv::createTrackbar("Roll", "camera settings", cameraSystemRotationRoll, 3600);
    cv::createTrackbar("Pitch", "camera settings", cameraSystemRotationPitch, 3600);
    cv::createTrackbar("Yaw", "camera settings", cameraSystemRotationYaw, 3600);
}

void Window::createCubeSettingsWindow(int32_t* cubeSystemTranslationX, int32_t* cubeSystemTranslationY, int32_t* cubeSystemTranslationZ,
                                         int32_t* cubeSystemRotationRoll, int32_t* cubeSystemRotationPitch, int32_t* cubeSystemRotationYaw) {
                                            
    cv::namedWindow("cube settings", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("X", "cube settings", cubeSystemTranslationX, 20000);
    cv::createTrackbar("Y", "cube settings", cubeSystemTranslationY, 20000);
    cv::createTrackbar("Z", "cube settings", cubeSystemTranslationZ, 20000);
    cv::createTrackbar("Roll", "cube settings", cubeSystemRotationRoll, 3600);
    cv::createTrackbar("Pitch", "cube settings", cubeSystemRotationPitch, 3600);
    cv::createTrackbar("Yaw", "cube settings", cubeSystemRotationYaw, 3600);
}
