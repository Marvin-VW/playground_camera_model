#include "GraphicsEngine.h"
#include "Shape.h"
#include "CameraModel.h"
#include "HomogenousTransformationMatrix.h"
#include <cmath>

#include <iostream>

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

int main(int argc, char** argv) {
    // Initialize the graphics engine
    GraphicsEngine* engine = GraphicsEngine::get();

    //init engine (window)
    engine->init();

    //instances of shape, camera and matrix
    Shape* shape = engine->createCube();
    CameraModel* camera = engine->createCamera(0.00452, 0.00254, 0.004, 1280, 720, 1280 / 2, 720 / 2);
    HomogenousTransformationMatrix* matrix = engine->init_matrices();

    //generate cube mesh
    const std::vector<triangle> mesh = shape->generate_mesh();


    // Start rendering loop
    while (true) {

        camera->resetCameraImage();

        engine->create_matrices();

        std::vector<triangle> world_mesh;
        std::vector<triangle> camera_mesh;

        // Apply V_T_Cube transformation (from world space to cube space)
        camera->transform(&camera->V_T_Cube, mesh, world_mesh);

        // Apply C_T_V transformation (from cube space to camera space)
        camera->transform(&camera->C_T_V, world_mesh, camera_mesh);

        camera->drawAllPoints(camera_mesh);
        camera->drawAllLines(camera_mesh);

        engine->renderFrame();

        if (cv::waitKey(10) == 27) { // Break loop on ESC key
            break;
        }
        
    }

    // Clean up and shut down
    engine->shutdown();

    return 0;
}
