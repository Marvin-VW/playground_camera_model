// Copyright (C) twyleg, 2024 Daniel-VW
#include <vector>
#include <chrono>

class FpsCounter {
public:
    FpsCounter(int filter_window_size = 1);
    void update();
    float get_fps() const;
    float get_fps_filtered() const;

private:
    int filter_window_size;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_timestamp;
    float fps;
    std::vector<float> fps_history;
};
