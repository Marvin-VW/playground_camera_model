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
    camera_model.cc
    camera_model.h
    homogeneous_transformation_matrix.cc
    homogeneous_transformation_matrix.h
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
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../
    PRIVATE ${OpenCV_INCLUDE_DIRS}
)
