# Horiba CPP SDK

[![ci](https://github.com/w-samuelgauthier/horiba_cpp_sdk/actions/workflows/ci.yml/badge.svg)](https://github.com/w-samuelgauthier/horiba_cpp_sdk/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/w-samuelgauthier/horiba_cpp_sdk/branch/main/graph/badge.svg)](https://codecov.io/gh/w-samuelgauthier/horiba_cpp_sdk)
[![CodeQL](https://github.com/w-samuelgauthier/horiba_cpp_sdk/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/w-samuelgauthier/horiba_cpp_sdk/actions/workflows/codeql-analysis.yml)

*horiba-cpp-sdk* is a C++ library that provides source code for the development with Horiba devices.

## Basic Usage Examples

1. Create a new folder with the following structure:

   ```text
   horiba_basic_example
   ├── cmake
   └── main.cpp
   ```
2. Get the `CPM.cmake` CMakeLists script from https://github.com/cpm-cmake/CPM.cmake/releases/tag/v0.38.7 and put it in the `cmake` folder

3. Create a CMakeLists.txt in the root folder:
   ```cmake
   cmake_minimum_required(VERSION 3.14)

   if (NOT DEFINED CMAKE_CXX_STANDARD)
     set(CMAKE_CXX_STANDARD 20)
   endif()

   project(horiba_cpp_example)

   include(cmake/CPM.cmake)

   CPMAddPackage("gh:ThatsTheEnd/horiba-cpp-sdk@0.1.0")


   add_executable(horiba_cpp_example main.cpp)
   target_link_libraries(horiba_cpp_example PRIVATE horiba_cpp_sdk::horiba_cpp_sdk)
   ```

4. Put the following into the `main.cpp`:

   ```c++
   int main() {
   // TODO
   return 0;
   }
   ```


5. Run `cmake -S . -B ./build -G Ninja` (replace `Ninja` with `"Unix Makefiles"` if you don't have Ninja or with `"Visual Studio 17 2022"` if you use Visual Studio 2022)

6. Run `cmake --build build`

7. Launch the example with `./build/horiba_cpp_example`

## More Details

 * [Dependency Setup](README_dependencies.md)
 * [Building Details](README_building.md)
 * [Troubleshooting](README_troubleshooting.md)
 * [Docker](README_docker.md)

## Credits
* Cmake project template from [lefticus](https://github.com/cpp-best-practices/cmake_template)
