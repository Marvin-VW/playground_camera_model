# extended_camera_model/CMakeLists.txt
cmake_minimum_required(VERSION 3.26.4)

project(extended_camera_model)

# Externals
add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest)

# Libs
add_subdirectory(libs/playground_camera_model/core)

# Apps
add_subdirectory(apps/playground_camera_model)

# Unit Tests
add_subdirectory(${CMAKE_SOURCE_DIR}/unit_tests)
 