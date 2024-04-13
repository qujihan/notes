cmake_minimum_required(VERSION 3.21)

project(template
    VERSION 0.0.1
    DESCRIPTION "Modern C++ Template"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
add_executable(main src/main.cc)