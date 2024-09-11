// Copyright (C) twyleg, 2024 Daniel-VW
#include "FPSCounter.h"

FpsCounter::FpsCounter(int filter_window_size)
    : filter_window_size(filter_window_size), fps(0.0f) {
    last_timestamp = std::chrono::high_resolution_clock::now();
}

void FpsCounter::update() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta_time = now - last_timestamp;
    last_timestamp = now;

    if (delta_time.count() > 0) {
        fps = 1.0f / delta_time.count();
    } else {
        fps = 0.0f;
    }

    fps_history.push_back(fps);
    if (fps_history.size() > filter_window_size) {
        fps_history.erase(fps_history.begin());
    }
}

float FpsCounter::get_fps() const {
    return fps;
}

float FpsCounter::get_fps_filtered() const {
    if (!fps_history.empty()) {
        float sum = 0;
        for (float f : fps_history) {
            sum += f;
        }
        return sum / fps_history.size();
    } else {
        return 0.0f;
    }
}
