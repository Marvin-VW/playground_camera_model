#include "Engine.h"
#include "CubeTracker.h"
#include "converter.h"
#include <opencv2/opencv.hpp>
#include "Shape.h"

int main() {


    int frame_width = 640;
    int frame_height = 480;

    Engine* engine = new Engine(frame_width, frame_height);
    CubeTracker cubeTracker(frame_width, frame_height);

    cv::Mat I_T_C = engine->renderer->cm->I_T_C;

    Converter cubeGen(I_T_C);

    bool running = true;
    while (running) {
        cv::Mat frame = cv::imread("/Users/vw67pfr/Desktop/playground_camera_model/cpp/cube_tracker/Engine/apps/playground_camera_model/5.jpg");

        std::vector<cv::Point2d> corners = cubeTracker.trackCube(frame);

        for (const auto& point : corners) {
            std::cout << point << std::endl;
        }

        double side_length = abs(corners[0].x - corners[1].x);

        cubeGen.setBasePoints(corners, side_length);

        cubeGen.computeCubePoints();

        std::vector<cv::Mat> cube_points = cubeGen.getCubePoints();
        for (const auto& point : cube_points) {
            std::cout << "3D Point: " << point.t() << std::endl;
        }

        int key = cv::waitKey(10);
        
        std::vector<triangle> mesh = cubeGen.computeMesh();

        engine->run(key, frame, mesh);
        engine->camera->resetCameraImage(frame);
        if (key == 27) {
            running = false;
        }
    }

    cv::destroyAllWindows();
    delete engine;

    return 0;
}