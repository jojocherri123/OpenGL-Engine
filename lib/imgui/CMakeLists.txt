# Define the library target
add_library(imgui STATIC
    imgui.h
    imgui_internal.h
    imstb_rectpack.h
    imgui.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3_loader.h
    backends/imgui_impl_sdl2.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_sdl2.h
    imstb_truetype.h
    imstb_textedit.h
    imgui_tables.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imconfig.h
    imgui_widgets.cpp
)

# Specify include directories for the library
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
