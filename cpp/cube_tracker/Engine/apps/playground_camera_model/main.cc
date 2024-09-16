#include "Engine.h"
#include <opencv2/opencv.hpp>

int main() {

    cv::Scalar lower_pink(140, 100, 100);
    cv::Scalar upper_pink(170, 255, 255);

    std::string video_path = "http://192.168.30.142:8443";
    cv::VideoCapture cap(video_path);

    cv::Mat frame, hls_frame, mask;

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file!" << std::endl;
        return -1;
    }

    Engine* engine = new Engine();
    
    bool running = true;
    while (running) {


        bool ret = cap.read(frame);

        if (!ret) {
            break;
        }

        cv::cvtColor(frame, hls_frame, cv::COLOR_BGR2HLS);

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

            cv::circle(frame, cv::Point(center_x, center_y), 5, cv::Scalar(255, 0, 0), -1);

        }
        cv::imshow("Pink Cube Tracker", frame);
        cv::imshow("Mask", mask);

        int key = cv::waitKey(10);

        engine->run(key);
        
        if (key == 27) {
            running = false;
        
        }
    }

    cap.release();
    cv::destroyAllWindows();


    delete engine;
    return 0;
}
