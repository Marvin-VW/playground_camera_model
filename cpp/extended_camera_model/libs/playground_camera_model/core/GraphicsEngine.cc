#include "GraphicsEngine.h"
#include "Shape.h"
#include "iostream"
#include "HomogenousTransformationMatrix.h"
#include "Window.h"
#include "CameraModel.h"


#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

bool GraphicsEngine::init() {
    // Initialize transformation parameters
    cameraSystemTranslationX = 10000;
    cameraSystemTranslationY = 10000;
    cameraSystemTranslationZ = 11000;

    cameraSystemRotationRoll = 2500;
    cameraSystemRotationPitch = 0;
    cameraSystemRotationYaw = 2700;

    cubeSystemTranslationX = 14000;
    cubeSystemTranslationY = 10000;
    cubeSystemTranslationZ = 11000;

    cubeSystemRotationRoll = 0;
    cubeSystemRotationPitch = 0;
    cubeSystemRotationYaw = 0;

    // Create GUI
    Window::createCameraSettingsWindow(&cameraSystemTranslationX, &cameraSystemTranslationY, &cameraSystemTranslationZ,
                                       &cameraSystemRotationRoll, &cameraSystemRotationPitch, &cameraSystemRotationYaw);
    Window::createCubeSettingsWindow(&cubeSystemTranslationX, &cubeSystemTranslationY, &cubeSystemTranslationZ,
                                     &cubeSystemRotationRoll, &cubeSystemRotationPitch, &cubeSystemRotationYaw);
    
    return true;
}


bool GraphicsEngine::release()
{


}


HomogenousTransformationMatrix* GraphicsEngine::init_matrices()
{

    ht = new HomogenousTransformationMatrix();

    cm->V_T_C = ht->createHomogeneousTransformationMatrix(0, 0, 0, 0, 0, 0);
    cm->C_T_V = cm->V_T_C.inv();
    cm->V_T_Cube = ht->createHomogeneousTransformationMatrix(2, 0, 1, 0, 0, 0);

    return ht;
}


CameraModel* GraphicsEngine::create_matrices()
{

    // Create camera to world matrix
    cm->V_T_C = ht->createHomogeneousTransformationMatrix(
        (cameraSystemTranslationX - 10000) / 1000.0,
        (cameraSystemTranslationY - 10000) / 1000.0,
        (cameraSystemTranslationZ - 10000) / 1000.0,
        DEG_TO_RAD(cameraSystemRotationRoll / 10.0),
        DEG_TO_RAD(cameraSystemRotationPitch / 10.0),
        DEG_TO_RAD(cameraSystemRotationYaw / 10.0));

    // Compute inverse (world to camera matrix)
    cm->C_T_V = cm->V_T_C.inv();

    // Create cube to world matrix
    cm->V_T_Cube = ht->createHomogeneousTransformationMatrix(
        (cubeSystemTranslationX - 10000) / 1000.0,
        (cubeSystemTranslationY - 10000) / 1000.0,
        (cubeSystemTranslationZ - 10000) / 1000.0,
        DEG_TO_RAD(cubeSystemRotationRoll / 10.0),
        DEG_TO_RAD(cubeSystemRotationPitch / 10.0),
        DEG_TO_RAD(cubeSystemRotationYaw / 10.0));

}


GraphicsEngine* GraphicsEngine::get()
{
    static GraphicsEngine engine;
    return &engine;
}

Shape* GraphicsEngine::createCube() {

    Shape* sp = new Shape();

    mesh = sp->generate_mesh();

    return sp;

}

CameraModel* GraphicsEngine::createCamera(  double sensorWidth,
                                            double sensorHeight,
                                            double focalLength,
                                            uint32_t resolutionX,
                                            uint32_t resolutionY,
                                            uint32_t u0,
                                            uint32_t v0) 
{

    cm = new CameraModel(sensorWidth, sensorHeight, focalLength, resolutionX, resolutionY, u0, v0);

    return cm;

}

void GraphicsEngine::renderFrame() {

        cv::imshow("image window", cm->getCameraImage());

}

bool GraphicsEngine::isRunning() {

}

void GraphicsEngine::shutdown() {
    std::cout << "Shutting down Graphics Engine" << std::endl;
    delete this;
}