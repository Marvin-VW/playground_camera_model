// Copyright (C) 2024 Marvin-VW
#include "GraphicsEngine.h"
#include "Shape.h"
#include "CameraModel.h"
#include "HomogenousTransformationMatrix.h"
#include "ClippingSpace.h"
#include <cmath>

#include <iostream>

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

int main(int argc, char** argv) {
    // Initialize the graphics engine
    GraphicsEngine* engine = new GraphicsEngine();

    //instances of shape, camera and matrix
    Shape* shape = engine->createCube();
    CameraModel* camera = engine->createCamera(0.00452, 0.00254, 0.004, 1280, 720, 1280 / 2, 720 / 2);
    HomogenousTransformationMatrix* matrix = engine->init_matrices();
    ClippingSpace* clipping = engine->init_clipping();

    //generate cube mesh
    const std::vector<triangle> mesh = shape->generate_mesh();


    // Start rendering loop
    while (true) {

        cv::Vec3f camera_vector_world = camera->getCameraVector(camera->V_T_C);

        std::cout << camera_vector_world << std::endl;

        camera->resetCameraImage();

        engine->create_matrices();

        std::vector<triangle> world_mesh;
        std::vector<triangle> camera_mesh;

        camera->transform(&camera->V_T_Cube, mesh, world_mesh);

        camera->transform(&camera->C_T_V, world_mesh, camera_mesh);

        //clipping
        std::vector<triangle> clipped_mesh;
        clipped_mesh = clipping->cubeInSpace(camera_mesh);


        camera->drawAllPoints(clipped_mesh);
        camera->drawAllLines(clipped_mesh);

        engine->update_fps();
        engine->renderFrame();

        if (cv::waitKey(10) == 27) { // Break loop on ESC key
            break;
        }
        
    }

    // Clean up and shut down
    engine->shutdown();

    return 0;
}
