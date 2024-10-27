cmake_minimum_required(VERSION 3.0.0)
project(OpenGLEngine VERSION 0.0.2)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

include_directories(include)

add_subdirectory(src)

add_subdirectory(lib/glad)
add_subdirectory(lib/imgui)
