#include "Engine.h"
#include <opencv2/opencv.hpp>

int main() {
    Engine* engine = new Engine();
    
    bool running = true;
    while (running) {

        int key = cv::waitKey(10);

        engine->run(key);
        
        if (key == 27) {
            running = false;
        }
    }

    delete engine;
    return 0;
}
