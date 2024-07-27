# Copyright (C) 2024 twyleg
import time
from typing import List


class FpsCounter:

    def __init__(self, filter_window_size=1):
        self.filter_window_size = filter_window_size
        self.last_timestamp = time.time()
        self.fps: float | None = None
        self.fps_history: List[float] = []

    def update(self) -> None:
        timestamp = time.time()
        delta_time = timestamp - self.last_timestamp
        self.last_timestamp = timestamp
        try:
            self.fps = 1.0 / delta_time
        except:
            self.fps = 0
        self.fps_history.append(self.fps)
        if len(self.fps_history) > self.filter_window_size:
            self.fps_history.pop(0)

    def get_fps(self) -> float:
        return self.fps

    def get_fps_filtered(self) -> float:
        if len(self.fps_history):
            return sum(self.fps_history) / len(self.fps_history)
        else:
            return 0
