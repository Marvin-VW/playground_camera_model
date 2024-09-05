[![Build status](https://github.com/twyleg/playground_camera_model/actions/workflows/unit_tests.yaml/badge.svg)]()
[![GitHub latest commit](https://badgen.net/github/last-commit/twyleg/playground_camera_model)](https://GitHub.com/twyleg/playground_camera_model/commit/)

# playground_camera_model

Simple CMake based template for a common C++ project.

## Dependencies

The template is based on the following components:

* CMake
* boost
* googletest
* fmt
* libopencv-dev

For a Debian/Ubuntu system, run the following command to install the dependencies:

	sudo apt install \
		git \
		cmake \
		g++ \
		libfmt-dev \
		libboost-all-dev \
		libopencv-dev

GoogleTest is pulled in as a git submodule to avoid problems with missing cmake files in sub distributions.

## Cloning the Repository

To clone the repository and initialize the submodules, execute the following commands:

```bash
git clone git@github.com:twyleg/playground_camera_model.git
cd playground_camera_model
git submodule update --init
```

## Building the Project

The project contains two models: a basic camera model and an extended camera model. Follow the respective instructions to build the desired model.

### Basic Camera Model

To build the basic camera model, perform the following steps:

1. Navigate to the basic camera model directory:

    ```bash
    cd cpp/basic_camera_model
    ```

2. Create a build directory and navigate into it:

    ```bash
    mkdir build
    cd build
    ```

3. Generate the build files with CMake:

    ```bash
    cmake ../
    ```

4. Compile the project:

    ```bash
    make
    ```

### Extended Camera Model

To build the extended camera model, follow these steps:

1. Navigate to the extended camera model directory:

    ```bash
    cd cpp/extended_camera_model
    ```

2. Create a build directory and navigate into it:

    ```bash
    mkdir build
    cd build
    ```

3. Generate the build files with CMake:

    ```bash
    cmake ../
    ```

4. Compile the project:

    ```bash
    make
    ```