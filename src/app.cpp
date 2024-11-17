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
#include <imgui_internal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "app.hpp"
#include "errorhandler.hpp"

App::App(WindowMain &windowMain, OpenGLContext &context) : windowMain(windowMain), fogColor(0.0f, 0.0f, 0.0f)
{
    windowMain.initFrameBuffer();
}

void App::init()
{

    backPack.loadModel("./assets/objects/sponza-scene/source/sponza/sponza.obj");
    lightSource.loadModel("./assets/objects/light.obj");

    shader.init("./shaders/vertex.glsl",
                "./shaders/fragment.glsl",
                "./shaders/geometry.glsl");

    lightShader.init("./shaders/light.vertex.glsl",
                     "./shaders/light.fragment.glsl");

    frameBufferShader.init("./shaders/fbo.vertex.glsl",
                           "./shaders/fbo.fragment.glsl");

    engineGui.setup(*windowMain.getWindow(), windowMain.OpenGLContext);
}

void App::run()
{

    SDL_WarpMouseInWindow(windowMain.getWindow(), windowMain.SCRNWidth / 2, windowMain.SCRNHeight / 2);
    while (!windowMain.Quit)
    {

        processInput();
        preDraw();

        engineGui.setupDrawWindow();

        draw();
        processLights();

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

        engineGui.createMenus(fogDensity, fogColor, lightSettings);

        engineGui.render();

        SDL_GL_SwapWindow(windowMain.getWindow());
    }
}

void App::processInput()
{
    static int mouseX = windowMain.SCRNWidth / 2;
    static int mouseY = windowMain.SCRNHeight / 2;
    static bool inputCapture = false;
    static bool show_context_menu = false;

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    Uint32 buttonState = SDL_GetMouseState(NULL, NULL);
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
        
        if (buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT)){
            std::cout << "Right menu should pop" << std::endl;
            // engineGui.rightClickMenu(x,y);
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
            camera.MouseLook(mouseX, mouseY);
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
            camera.MoveForward(0.1f);
        }
        if (state[SDL_SCANCODE_S])
        {
            camera.MoveBackward(0.1f);
        }

        if (state[SDL_SCANCODE_D])
        {
            camera.MoveLeft(0.1f);
        }
        if (state[SDL_SCANCODE_A])
        {
            camera.MoveRight(0.1f);
        }
        if (state[SDL_SCANCODE_E])
        {
            camera.MoveUp(0.1f);
        }
        if (state[SDL_SCANCODE_Q])
        {
            camera.MoveDown(0.1f);
        }
    }
}

void App::preDraw()
{
    // shader.use();
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

    shader.apply(camera, lightSettings, fogDensity, fogColor, (float)windowMain.SCRNWidth / (float)windowMain.SCRNHeight);
}

void App::draw()
{

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    backPack.Draw(shader);
    glUseProgram(0);
}

void App::processLights()
{

    int numOfPointLight = lightSettings.getPointLightPositions().size();

    int numOfSpotLight = lightSettings.getSpotLightPositions().size();
    // int numOfDirLight = lightSettings.getDirectionalLightAngles().size();

    lightShader.use();
    if (lightSettings.getShowLights())
    {

        for (int i = 0; i < numOfPointLight; i++)
        {
            lightShader.applyLight(camera, lightSettings, lightSettings.getPointLightPositions()[i], lightSettings.getPointLightColors()[i], (float)windowMain.SCRNWidth / (float)windowMain.SCRNHeight);
            lightSource.Draw(lightShader);
        };

        for (int i = 0; i < numOfSpotLight; i++)
        {
            lightShader.applyLight(camera, lightSettings, lightSettings.getSpotLightPositions()[i], lightSettings.getSpotLightColors()[i], (float)windowMain.SCRNWidth / (float)windowMain.SCRNHeight);
            lightSource.Draw(lightShader);
        };
    }
}

void App::destroy()
{
    engineGui.clean();
    shader.deleteShader();
    lightShader.deleteShader();
    frameBufferShader.deleteShader();
    SDL_DestroyWindow(windowMain.getWindow());
    SDL_Quit();
}
