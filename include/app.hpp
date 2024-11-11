#pragma once

#include <glad.h>
#include <glm/vec3.hpp>

#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "gui.hpp"
#include "window.hpp"
#include "lightsettings.hpp"

class WindowMain;
class OpenGLContext;

class App
{
    LightSettings lightSettings;
    WindowMain &windowMain;
    Camera camera;
    GLuint texture;
    Shader shader;
    Model backPack;
    EngineGUI engineGui;
    Shader lightShader;
    Model lightSource;
    Shader frameBufferShader;
    float fogDensity;
    glm::vec3 fogColor;

    void processInput();
    void preDraw();
    void draw();
    void processLights();

public:
    App(WindowMain &windowMain, OpenGLContext &context);

    void init();
    void run();
    void destroy();
};