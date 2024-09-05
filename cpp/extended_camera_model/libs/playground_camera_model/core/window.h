#ifndef WINDOW_H
#define WINDOW_H

#include <opencv2/opencv.hpp>

class Window {
public:

    static void createCameraSettingsWindow(int32_t* cameraSystemTranslationX, int32_t* cameraSystemTranslationY, int32_t* cameraSystemTranslationZ,
                                           int32_t* cameraSystemRotationRoll, int32_t* cameraSystemRotationPitch, int32_t* cameraSystemRotationYaw);
                                           
    static void createCubeSettingsWindow(int32_t* cubeSystemTranslationX, int32_t* cubeSystemTranslationY, int32_t* cubeSystemTranslationZ,
                                         int32_t* cubeSystemRotationRoll, int32_t* cubeSystemRotationPitch, int32_t* cubeSystemRotationYaw);
};

#endif