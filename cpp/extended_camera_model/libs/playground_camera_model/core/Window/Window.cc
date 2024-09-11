// Copyright (C) 2024 Daniel-VW
#include "Window.h"
#include <opencv2/opencv.hpp>
#include <cmath>

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
                                         int32_t* cubeSystemRotationRoll, int32_t* cubeSystemRotationPitch, int32_t* cubeSystemRotationYaw, int32_t* cubeSystemScale) {
                                            
    cv::namedWindow("cube settings", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("X", "cube settings", cubeSystemTranslationX, 20000);
    cv::createTrackbar("Y", "cube settings", cubeSystemTranslationY, 20000);
    cv::createTrackbar("Z", "cube settings", cubeSystemTranslationZ, 20000);
    cv::createTrackbar("Roll", "cube settings", cubeSystemRotationRoll, 3600);
    cv::createTrackbar("Pitch", "cube settings", cubeSystemRotationPitch, 3600);
    cv::createTrackbar("Yaw", "cube settings", cubeSystemRotationYaw, 3600);
    cv::createTrackbar("Scale", "cube settings", cubeSystemScale, 5);
}

void Window::handleMovement(int key, int32_t* cameraSystemTranslationX, int32_t* cameraSystemTranslationY, int32_t* cameraSystemTranslationZ,
                            int32_t* cameraSystemRotationRoll, int32_t* cameraSystemRotationPitch, int32_t* cameraSystemRotationYaw) {
    Direction dir;
    switch (key) {
        case 'd': dir = FORWARD; break;
        case 'a': dir = BACKWARD; break;
        case 'w': dir = LEFT; break;
        case 's': dir = RIGHT; break;
        case 'q': dir = DOWN; break;
        case 'e': dir = UP; break;
        default: return;
    }

    moveCamera(dir, speed, cameraSystemTranslationX, cameraSystemTranslationY, cameraSystemTranslationZ, *cameraSystemRotationYaw, *cameraSystemRotationPitch);
}

void Window::updateCameraVectors(int32_t cameraSystemRotationYaw, int32_t cameraSystemRotationPitch) {
    const double yaw = (cameraSystemRotationYaw / 10.0) * (M_PI / 180.0);
    const double pitch = (cameraSystemRotationPitch / 10.0) * (M_PI / 180.0);
    
    forwardX = std::cos(pitch) * std::cos(yaw);
    forwardY = std::cos(pitch) * std::sin(yaw);
    forwardZ = std::sin(pitch);
    
    rightX = std::sin(yaw);
    rightY = -std::cos(yaw);
}

void Window::moveCamera(Direction direction, int speed, int32_t* cameraSystemTranslationX, int32_t* cameraSystemTranslationY, int32_t* cameraSystemTranslationZ,
                        int32_t cameraSystemRotationYaw, int32_t cameraSystemRotationPitch) {

    updateCameraVectors(cameraSystemRotationYaw, cameraSystemRotationPitch); 
    if (direction == FORWARD) {
        *cameraSystemTranslationX += static_cast<int>(forwardX * speed);
        *cameraSystemTranslationY += static_cast<int>(forwardY * speed);
        *cameraSystemTranslationZ += static_cast<int>(forwardZ * speed);
    } else if (direction == BACKWARD) {
        *cameraSystemTranslationX -= static_cast<int>(forwardX * speed);
        *cameraSystemTranslationY -= static_cast<int>(forwardY * speed);
        *cameraSystemTranslationZ -= static_cast<int>(forwardZ * speed);
    } else if (direction == LEFT) {
        *cameraSystemTranslationX -= static_cast<int>(rightX * speed);
        *cameraSystemTranslationY -= static_cast<int>(rightY * speed);
    } else if (direction == RIGHT) {
        *cameraSystemTranslationX += static_cast<int>(rightX * speed);
        *cameraSystemTranslationY += static_cast<int>(rightY * speed);
    } else if (direction == UP) {
        *cameraSystemTranslationZ += static_cast<int>(1 * speed);
    } else if (direction == DOWN) {
        *cameraSystemTranslationZ -= static_cast<int>(1 * speed);
    }

    // Clamping values
    *cameraSystemTranslationX = std::clamp(*cameraSystemTranslationX, 0, 20000);
    *cameraSystemTranslationY = std::clamp(*cameraSystemTranslationY, 0, 20000);
    *cameraSystemTranslationZ = std::clamp(*cameraSystemTranslationZ, 0, 20000);
    
    // Update trackbars
    cv::setTrackbarPos("X", "camera settings", *cameraSystemTranslationX);
    cv::setTrackbarPos("Y", "camera settings", *cameraSystemTranslationY);
    cv::setTrackbarPos("Z", "camera settings", *cameraSystemTranslationZ);
}

