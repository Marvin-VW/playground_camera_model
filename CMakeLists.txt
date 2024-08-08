# Copyright (C) 2024 twyleg
cmake_minimum_required(VERSION 3.26.4)

project(playground_camera_model)

# Externals
add_subdirectory(external/googletest/)

# Libs
add_subdirectory(libs/playground_camera_model/core/)

# Apos
add_subdirectory(apps/playground_camera_model)

# Unit-Test
add_subdirectory(unit_tests/)
