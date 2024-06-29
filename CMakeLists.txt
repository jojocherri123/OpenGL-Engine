cmake_minimum_required(VERSION 3.0.0)
project(OpenGLEngine VERSION 0.0.2)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(OpenGLEngine 
    src/main.cpp
    src/includes/glad/glad.h
    src/includes/glad/glad.c
    
    src/includes/imgui/imgui_demo.cpp
    src/includes/imgui/imgui_draw.cpp
    src/includes/imgui/imgui_internal.h
    src/includes/imgui/imgui_tables.cpp
    src/includes/imgui/imgui_widgets.cpp
    src/includes/imgui/imgui.h
    src/includes/imgui/imgui.cpp
    src/includes/imgui/imstb_rectpack.h
    src/includes/imgui/imstb_textedit.h
    src/includes/imgui/imstb_truetype.h
    src/includes/imgui/backends/imgui_impl_sdl2.h
    src/includes/imgui/backends/imgui_impl_sdl2.cpp
    src/includes/imgui/backends/imgui_impl_opengl3.h
    src/includes/imgui/backends/imgui_impl_opengl3.cpp

    src/includes/gui/gui.hpp
    src/includes/gui/gui.cpp

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
    OpenGL::GL
    assimp
    stb
)

