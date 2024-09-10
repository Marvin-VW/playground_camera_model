// Copyright (C) 2024 Marvin-VW
#include "GraphicsEngine.h"
#include "Shape.h"
#include "CameraModel.h"
#include "HomogenousTransformationMatrix.h"
#include "ClippingSpace.h"
#include "Vectors.h"
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
    Vectors* vec = engine->init_vector();

    //generate cube mesh
    std::vector<triangle> mesh = shape->generate_mesh();


    // Start rendering loop
    while (true) {

        cv::Vec3f camera_vector_world = camera->getCameraVector(camera->V_T_C);

        camera->resetCameraImage();

        engine->create_matrices();

        for (auto& tri : mesh) {

            camera->world_transform(&camera->V_T_Cube, &tri);

            camera->camera_transform(&camera->C_T_V, &tri);

            std::tuple<cv::Mat, cv::Mat> result = vec->normal(tri, 0.5f);

            // Unpack the tuple
            cv::Mat normal_start = std::get<0>(result);
            cv::Mat normal_end = std::get<1>(result);

            //normal to camera
            cv::Mat normal_start_camera = camera->C_T_V * normal_start;
            cv::Mat normal_end_camera = camera->C_T_V * normal_end;

            camera->drawCameraImageArrow(normal_start_camera, normal_end_camera);

        }

        //clipping
        std::vector<triangle> clipped_mesh;
        clipped_mesh = clipping->cubeInSpace(&mesh);

        camera->drawAllPoints(&clipped_mesh);
        camera->drawAllLines(&clipped_mesh);

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
