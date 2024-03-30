cmake_minimum_required(VERSION 3.0.0)
project(OpenGLEngine VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(OpenGLEngine 
    src/main.cpp
    src/glad.h
    src/glad.c
    src/camera.hpp
    src/camera.cpp
    )

target_link_libraries(OpenGLEngine
    SDL2
    glfw
    OpenGL::GL
)

