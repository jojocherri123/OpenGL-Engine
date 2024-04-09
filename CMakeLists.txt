cmake_minimum_required(VERSION 3.0.0)
project(OpenGLEngine VERSION 0.0.2)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(OpenGLEngine 
    src/main.cpp
    src/includes/glad/glad.h
    src/includes/glad/glad.c

    src/includes/camera/camera.hpp
    src/includes/camera/camera.cpp

    src/includes/importModel/mesh.hpp
    src/includes/importModel/model.hpp
    src/includes/importModel/model.cpp

    src/includes/shader/shader.hpp
    
    )

target_link_libraries(OpenGLEngine
    SDL2
    SDL2_image
    glfw
    OpenGL::GL
    assimp
    stb
)

