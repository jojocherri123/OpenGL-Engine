#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <sstream>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "gui.hpp"
#include "window.hpp"
#include "lightsettings.hpp"

LightSettings lightSettings;
WindowMain windowMain;
Camera gCamera;
GLuint gtexture;
Shader shader;
Model backPack;
EngineGUI engineGui;
Shader Lightshader;
Model LightSource;
Shader frameBufferShader;

float FogDensity;
glm::vec3 FogColor;

static void GLclearAllErrors()
{
    while (glGetError() != GL_NO_ERROR)
    {
    }
}

static bool GLcheckErrorStatus(const char *function, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL ERROR: " << error
                  << "\tLine: " << line
                  << "\tFunction: " << function << std::endl;

        return true;
    }
    return false;
}

#define GLCheckError(x) \
    GLclearAllErrors(); \
    x;                  \
    GLcheckErrorStatus(#x, __LINE__);

void GetOpengGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitializeProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL COUDLNT LOAD" << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    windowMain.GraphicsWinow = SDL_CreateWindow("Shade Line Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                                windowMain.SCRNWidth, windowMain.SCRNHeight,
                                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (windowMain.GraphicsWinow == nullptr)
    {
        std::cout << "Couldn't create window" << std::endl;
        exit(1);
    }

    windowMain.OpenGLContext = SDL_GL_CreateContext(windowMain.GraphicsWinow);
    if (windowMain.OpenGLContext == nullptr)
    {
        std::cout << "Couldn't create opengl context" << std::endl;
        exit(1);
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Couldn't initialize glad" << std::endl;
        exit(1);
    }

    SDL_SetWindowIcon(windowMain.GraphicsWinow, windowMain.iconSurface);

    GetOpengGLVersionInfo();
}

void Input()
{
    static int mouseX = windowMain.SCRNWidth / 2;
    static int mouseY = windowMain.SCRNHeight / 2;
    static bool inputCapture = false;

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        engineGui.processEvent(e);

        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_m && inputCapture == false)
            {
                inputCapture = true;
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
            else if (e.key.keysym.sym == SDLK_m && inputCapture == true)
            {
                inputCapture = false;
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
        }

        if (e.type == SDL_QUIT)
        {
            std::cout << "Exited Succesfully" << std::endl;
            windowMain.Quit = true;
        }

        if (e.type == SDL_MOUSEMOTION && inputCapture == true)
        {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gCamera.MouseLook(mouseX, mouseY);
        }
    }

    if (state[SDL_SCANCODE_ESCAPE])
    {
        std::cout << "Exited Succesfully" << std::endl;
        windowMain.Quit = true;
    }

    if (inputCapture)
    {
        if (state[SDL_SCANCODE_W])
        {
            gCamera.MoveForward(0.1f);
        }
        if (state[SDL_SCANCODE_S])
        {
            gCamera.MoveBackward(0.1f);
        }

        if (state[SDL_SCANCODE_D])
        {
            gCamera.MoveLeft(0.1f);
        }
        if (state[SDL_SCANCODE_A])
        {
            gCamera.MoveRight(0.1f);
        }
        if (state[SDL_SCANCODE_E])
        {
            gCamera.MoveUp(0.1f);
        }
        if (state[SDL_SCANCODE_Q])
        {
            gCamera.MoveDown(0.1f);
        }
    }
}

void PreDraw()
{

    glViewport(0, 0, windowMain.SCRNWidth, windowMain.SCRNHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, windowMain.fbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_FRAMEBUFFER_SRGB);

    shader.use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f,0.0f,0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

    shader.setMatrix4FV("u_ModelMatrix", model);

    glm::mat4 view = gCamera.getViewMatrix();
    shader.setMatrix4FV("u_ViewMatrix", view);

    glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)windowMain.SCRNWidth / (float)windowMain.SCRNHeight, 0.1f, 1000.0f);
    shader.setMatrix4FV("u_Projection", perspective);

    shader.setFloat4("u_LightColor", lightSettings.getLightColor().x, lightSettings.getLightColor().y, lightSettings.getLightColor().z, lightSettings.getLightColor().w);

    shader.setFloat3v("pointLights[0].position", lightSettings.getPointLightPositions()[0]);
    shader.setFloat3v("pointLights[1].position", lightSettings.getPointLightPositions()[1]);
    shader.setFloat3v("directionalLight[0].angle", lightSettings.getDirectionalLightAngles()[0]);
    shader.setFloat3v("spotLight[0].position", lightSettings.getSpotLightPositions()[0]);
    shader.setFloat3v("spotLight[0].angle", lightSettings.getSpotLightAngles()[0]);
    shader.setFloat("FogDensity", FogDensity);
    shader.setFloat3v("FogColor", FogColor);

    shader.setFloat3("u_CamPos", gCamera.mEye.x, gCamera.mEye.y, gCamera.mEye.z);

    shader.setFloat("material.shininess", 8.0f);
}

void Draw()
{

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    backPack.Draw(shader);
    glUseProgram(0);
}

void Light()
{

    Lightshader.use();

    int numOfPointLight = lightSettings.getPointLightPositions().size();
    int numOfSpotLight = lightSettings.getSpotLightPositions().size();
    // shader.setInt("numOfPointLight", numOfPointLight);
    // shader.setInt("NR_POINT_LIGHTS", numOfPointLight);
    // shader.setInt("NR_SPT_LIGHTS", numOfSpotLight);

    for (int i = 0; i < numOfPointLight; i++)
    {

        glm::mat4 model = glm::translate(glm::mat4(1.0f), lightSettings.getPointLightPositions()[i]);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        Lightshader.setMatrix4FV("u_ModelMatrixLight", model);

        glm::mat4 view = gCamera.getViewMatrix();
        Lightshader.setMatrix4FV("u_ViewMatrixLight", view);

        glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)windowMain.SCRNWidth / (float)windowMain.SCRNHeight, 0.1f, 1000.0f);
        Lightshader.setMatrix4FV("u_ProjectionLight", perspective);

        Lightshader.setFloat4("u_LightColor", lightSettings.getLightColor().x, lightSettings.getLightColor().y, lightSettings.getLightColor().z, lightSettings.getLightColor().w);

        LightSource.Draw(Lightshader);
    };

    for (int i = 0; i < numOfSpotLight; i++)
    {

        glm::mat4 model = glm::translate(glm::mat4(1.0f), lightSettings.getSpotLightPositions()[i]);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        Lightshader.setMatrix4FV("u_ModelMatrixLight", model);

        glm::mat4 view = gCamera.getViewMatrix();
        Lightshader.setMatrix4FV("u_ViewMatrixLight", view);

        glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)windowMain.SCRNWidth / (float)windowMain.SCRNHeight, 0.1f, 1000.0f);
        Lightshader.setMatrix4FV("u_ProjectionLight", perspective);

        Lightshader.setFloat4("u_LightColor", lightSettings.getLightColor().x, lightSettings.getLightColor().y, lightSettings.getLightColor().z, lightSettings.getLightColor().w);

        LightSource.Draw(Lightshader);
    };
}

void MainLoop()
{

    SDL_WarpMouseInWindow(windowMain.GraphicsWinow, windowMain.SCRNWidth / 2, windowMain.SCRNHeight / 2);
    while (!windowMain.Quit)
    {

        Input();
        PreDraw();

        engineGui.setupDrawWindow();

        Draw();
        Light();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Draw the framebuffer rectangle

        static float gamma = 2.2f;

        frameBufferShader.use();
        frameBufferShader.set1i("screenTexture", 0);
        frameBufferShader.setFloat("gamma", gamma);
        glBindVertexArray(windowMain.rectVAO);
        glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
        glBindTexture(GL_TEXTURE_2D, windowMain.frameBufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::ShowDemoWindow();

        engineGui.createMenus(FogDensity, FogColor, lightSettings);

        engineGui.render();

        SDL_GL_SwapWindow(windowMain.GraphicsWinow);
    }
}

void CleanUp()
{
    engineGui.clean();
    shader.deleteShader();
    Lightshader.deleteShader();
    frameBufferShader.deleteShader();
    SDL_DestroyWindow(windowMain.GraphicsWinow);
    SDL_Quit();
}

void frameBuffer()
{

    float rectangleVertices[] = {
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &windowMain.rectVAO);
    glGenBuffers(1, &windowMain.rectVBO);
    glBindVertexArray(windowMain.rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowMain.rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glGenFramebuffers(1, &windowMain.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, windowMain.fbo);

    glGenTextures(1, &windowMain.frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, windowMain.frameBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowMain.SCRNWidth, windowMain.SCRNHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, windowMain.frameBufferTexture, 0);

    glGenRenderbuffers(1, &windowMain.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, windowMain.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowMain.SCRNWidth, windowMain.SCRNHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, windowMain.rbo);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: " << fboStatus << endl;
}

int main()
{

    InitializeProgram();

    frameBuffer();
    // stbi_set_flip_vertically_on_load(true);

    backPack.loadModel("./assets/objects/sponza-scene/source/sponza/sponza.obj");
    LightSource.loadModel("./assets/objects/light.obj");

    shader.init("./shaders/vertex.glsl",
                "./shaders/geometry.glsl",
                "./shaders/fragment.glsl");

    Lightshader.init("./shaders/light.vertex.glsl",
                     "./shaders/light.geo.glsl",
                     "./shaders/light.fragment.glsl");

    frameBufferShader.init("./shaders/fbo.vertex.glsl",
                           "./shaders/fbo.geo.glsl",
                           "./shaders/fbo.fragment.glsl");

    engineGui.setup(*windowMain.GraphicsWinow, windowMain.OpenGLContext);

    MainLoop();
    CleanUp();
    return 0;
}