cmake_minimum_required(VERSION 3.0.0)
project(OpenGLEngine VERSION 0.0.2)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(OpenGLEngine
    src/includes/glad/glad.c
    src/includes/glad/glad.h
    src/includes/imgui/backends/imgui_impl_opengl3.cpp
    src/includes/imgui/backends/imgui_impl_opengl3.h
    src/includes/imgui/backends/imgui_impl_sdl2.cpp
    src/includes/imgui/backends/imgui_impl_sdl2.h
    src/includes/imgui/imgui_demo.cpp
    src/includes/imgui/imgui_draw.cpp
    src/includes/imgui/imgui_internal.h
    src/includes/imgui/imgui_tables.cpp
    src/includes/imgui/imgui_widgets.cpp
    src/includes/imgui/imgui.cpp
    src/includes/imgui/imgui.h
    src/includes/imgui/imstb_rectpack.h
    src/includes/imgui/imstb_textedit.h
    src/includes/imgui/imstb_truetype.h

    src/includes/camera/camera.cpp
    src/includes/camera/camera.hpp
    src/includes/gui/gui.cpp
    src/includes/gui/gui.hpp
    src/includes/importModel/mesh.hpp
    src/includes/importModel/model.cpp
    src/includes/importModel/model.hpp
    src/includes/light/lightsettings.hpp
    src/includes/shader/shader.cpp
    src/includes/shader/shader.hpp
    src/includes/window/window.hpp
    src/main.cpp

)



target_link_libraries(OpenGLEngine
    SDL2
    SDL2_image
    OpenGL::GL
    assimp
    stb
)

