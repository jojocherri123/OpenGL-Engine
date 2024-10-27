cmake_minimum_required(VERSION 3.0.0)
project(OpenGLEngine VERSION 0.0.2)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(OpenGLEngine ${SRC_FILES})

target_link_libraries(OpenGLEngine
    SDL2
    SDL2_image
    OpenGL::GL
    assimp
    stb

    glad
    imgui
)
