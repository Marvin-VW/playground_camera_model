// Copyright (C) 2024 Daniel-VW, Marvin-VW, twyleg
#include "Window.h"
#include <opencv2/opencv.hpp>
#include <cmath>

int prevMouseX = -1;
int prevMouseY = -1;
bool pressed = false;
bool rightmode = false;

void Window::createCameraSettingsWindow()
{

    cv::namedWindow("camera settings", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("X", "camera settings", &cameraSystemTranslationX, 20000);
    cv::createTrackbar("Y", "camera settings", &cameraSystemTranslationY, 20000);
    cv::createTrackbar("Z", "camera settings", &cameraSystemTranslationZ, 20000);
    cv::createTrackbar("Roll", "camera settings", &cameraSystemRotationRoll, 3600);
    cv::createTrackbar("Pitch", "camera settings", &cameraSystemRotationPitch, 3600);
    cv::createTrackbar("Yaw", "camera settings", &cameraSystemRotationYaw, 3600);
}

void Window::createCubeSettingsWindow() {
                                            
    cv::namedWindow("cube settings", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("X", "cube settings", &cubeSystemTranslationX, 20000);
    cv::createTrackbar("Y", "cube settings", &cubeSystemTranslationY, 20000);
    cv::createTrackbar("Z", "cube settings", &cubeSystemTranslationZ, 20000);
    cv::createTrackbar("Roll", "cube settings", &cubeSystemRotationRoll, 3600);
    cv::createTrackbar("Pitch", "cube settings", &cubeSystemRotationPitch, 3600);
    cv::createTrackbar("Yaw", "cube settings", &cubeSystemRotationYaw, 3600);
    cv::createTrackbar("Scale", "cube settings", &cubeSystemScale, 5);
    cv::createTrackbar("Show Normals", "cube settings", &cubeSystemNormals, 1);
    cv::createTrackbar("Show Points", "cube settings", &cubeSystemPoints, 1);
    cv::createTrackbar("Show Faces", "cube settings", &cubeSystemFaces, 1);
}

void Window::handleMovement(int key) {
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

    moveCamera(dir, speed);
}

void Window::updateCameraVectors() {
    const double yaw = (cameraSystemRotationYaw / 10.0) * (M_PI / 180.0);
    const double pitch = (cameraSystemRotationPitch / 10.0) * (M_PI / 180.0);
    
    forwardX = std::cos(pitch) * std::cos(yaw);
    forwardY = std::cos(pitch) * std::sin(yaw);
    forwardZ = std::sin(pitch);
    
    rightX = std::sin(yaw);
    rightY = -std::cos(yaw);
}

void Window::moveCamera(Direction direction, int speed) {

    updateCameraVectors(); 
    if (direction == FORWARD) {
        cameraSystemTranslationX += static_cast<int>(forwardX * speed);
        cameraSystemTranslationY += static_cast<int>(forwardY * speed);
        cameraSystemTranslationZ += static_cast<int>(forwardZ * speed);
    } else if (direction == BACKWARD) {
        cameraSystemTranslationX -= static_cast<int>(forwardX * speed);
        cameraSystemTranslationY -= static_cast<int>(forwardY * speed);
        cameraSystemTranslationZ -= static_cast<int>(forwardZ * speed);
    } else if (direction == LEFT) {
        cameraSystemTranslationX -= static_cast<int>(rightX * speed);
        cameraSystemTranslationY -= static_cast<int>(rightY * speed);
    } else if (direction == RIGHT) {
        cameraSystemTranslationX += static_cast<int>(rightX * speed);
        cameraSystemTranslationY += static_cast<int>(rightY * speed);
    } else if (direction == UP) {
        cameraSystemTranslationZ += static_cast<int>(1 * speed);
    } else if (direction == DOWN) {
        cameraSystemTranslationZ -= static_cast<int>(1 * speed);
    }

    // Clamping values
    cameraSystemTranslationX = std::clamp(cameraSystemTranslationX, 0, 20000);
    cameraSystemTranslationY = std::clamp(cameraSystemTranslationY, 0, 20000);
    cameraSystemTranslationZ = std::clamp(cameraSystemTranslationZ, 0, 20000);
    
    // Update trackbars
    cv::setTrackbarPos("X", "camera settings", cameraSystemTranslationX);
    cv::setTrackbarPos("Y", "camera settings", cameraSystemTranslationY);
    cv::setTrackbarPos("Z", "camera settings", cameraSystemTranslationZ);
}

// Callback function to handle mouse events
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        pressed = true;
        prevMouseX = x;
        prevMouseY = y;
    }

    else if (event == cv::EVENT_LBUTTONUP)
    {
        pressed = false;
    }

    else if (event == cv::EVENT_RBUTTONDOWN)
    {
        rightmode = true;
    }

    else if (event == cv::EVENT_RBUTTONDBLCLK)
    {
        rightmode = false;
        prevMouseX = x;
        prevMouseY = y;
    }
    
    else if (event == cv::EVENT_MOUSEMOVE) {
        if (pressed == true || rightmode == true){
        Window* window = static_cast<Window*>(userdata);

        if (prevMouseX == -1 || prevMouseY == -1) {
            prevMouseX = x;
            prevMouseY = y;
            return;
        }

        int deltaX = x - prevMouseX;
        int deltaY = y - prevMouseY;

        prevMouseX = x;
        prevMouseY = y;

        window->moveCameraWithMouse(deltaX, deltaY);
        }
    }
    
}

void Window::mouseMoveCamera() {
    cv::setMouseCallback("image window", mouseCallback, this);
}

// Implement the method to adjust camera based on mouse delta
void Window::moveCameraWithMouse(int deltaX, int deltaY) {

    std::cout << deltaX << std::endl;
    std::cout << deltaY << std::endl;
    
    cameraSystemRotationYaw += deltaX;
    cameraSystemRotationRoll -= -deltaY;

    // Update the OpenCV trackbars with the new values
    cv::setTrackbarPos("Yaw", "camera settings", cameraSystemRotationYaw);
    cv::setTrackbarPos("Pitch", "camera settings", cameraSystemRotationPitch);
}
