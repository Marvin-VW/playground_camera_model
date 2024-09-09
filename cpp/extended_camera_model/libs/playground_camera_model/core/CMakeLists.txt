# Copyright (C) 2024 twyleg
set(NAMESPACE playground_camera_model)
set(MODULE core)

set(TARGET_NAME "${NAMESPACE}_${MODULE}")

#
# find packages
#
find_package(fmt REQUIRED)
find_package(Boost COMPONENTS REQUIRED system)
find_package(OpenCV REQUIRED)

#
# add source files to target
#
add_library(${TARGET_NAME}
    Camera/CameraModel.cc
    Camera/CameraModel.h
    Matrix/HomogenousTransformationMatrix.cc
    Matrix/HomogenousTransformationMatrix.h
    Shape/Shape.cc
    Shape/Shape.h
    Window/Window.cc
    Window/Window.h
    FPSCounter/FPSCounter.cc
    FPSCounter/FPSCounter.h
    GraphicsEngine.cc
    GraphicsEngine.h
    ClippingSpace/ClippingSpace.cc
    ClippingSpace/ClippingSpace.h
)

add_library("${NAMESPACE}::${MODULE}" ALIAS ${TARGET_NAME})

#
# set target properties
#
set_target_properties(${TARGET_NAME}
    PROPERTIES
    CXX_STANDARD 17
    INCLUDE_CURRENT_DIR ON
)

#
# link against libs
#
target_link_libraries(${TARGET_NAME}
    fmt::fmt
    Boost::system
    ${OpenCV_LIBS}
)

#
# add parent folder to targets include directories
#
target_include_directories(${TARGET_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Camera
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Matrix
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shape
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Window
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/FPSCounter
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ClippingSpace
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
    PRIVATE ${OpenCV_INCLUDE_DIRS}
)
