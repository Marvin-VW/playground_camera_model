#include "Engine.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>



int main() {

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }


    cv::Scalar lower_pink(130, 130, 130);
    cv::Scalar upper_pink(255, 255, 255);

    cv::Mat hls_frame, mask;

    int frame_width = 640;
    int frame_height = 480;
    

    Engine* engine = new Engine(frame_width, frame_height);


    bool running = true;

    while (running) {


        cv::Mat frame = cv::imread("/Users/vw67pfr/Desktop/playground_camera_model/cpp/cube_tracker/Engine/apps/playground_camera_model/1.jpg");

        double relativ_x;
        double relativ_y;

        cv::cvtColor(frame, hls_frame, cv::COLOR_RGB2HLS);

        cv::inRange(hls_frame, lower_pink, upper_pink, mask);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        if (!contours.empty()) {
            auto largest_contour = *std::max_element(contours.begin(), contours.end(),
                                                     [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                                                         return cv::contourArea(a) < cv::contourArea(b);
                                                     });

            cv::Rect bounding_rect = cv::boundingRect(largest_contour);

            cv::rectangle(frame, bounding_rect, cv::Scalar(0, 255, 0), 2);

            int center_x = bounding_rect.x + bounding_rect.width / 2;
            int center_y = bounding_rect.y + bounding_rect.height / 2;

            relativ_x = static_cast<double>(center_x) / frame_width;
            relativ_y = static_cast<double>(center_y) / frame_height;
                        
            //std::cout << "Frame width: " << static_cast<double>(frame_width)/frame_height << ", Frame height: " << frame_height << std::endl;

            cv::circle(frame, cv::Point(center_x, center_y), 5, cv::Scalar(255, 0, 0), -1);

        }
        cv::imshow("Mask", mask);

        double engine_x = relativ_x - 0.5;
        double engine_y = relativ_y - 0.5;



        engine->renderer->window.cubeSystemTranslationY = (10000 - (engine_x*4500));
        engine->renderer->window.cubeSystemTranslationZ = (10000 - (engine_y*(6000)*(static_cast<double>(frame_width)/frame_height)));


        int key = cv::waitKey(0);


        engine->run(key, frame);
        
        if (key == 27) {
            running = false;
        
        }
    }

    cv::destroyAllWindows();


    delete engine;


    return 0;
}
