cmake_minimum_required(VERSION 3.26)
project(proj)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})
include_directories(include)
include_directories(libs)
include_directories(res)
include_directories(res/3d_models)

set(SOURCE_DIR src)
file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)
add_library(proj_lib ${SOURCE_FILES}
        src/Scene.cpp)

add_executable(proj src/main.cpp
        include/Scene.h)

target_link_libraries(proj_lib glfw GLEW::GLEW -lGL ${ASSIMP_LIBRARIES})
target_link_libraries(proj proj_lib)
