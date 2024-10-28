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
