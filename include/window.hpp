#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SDLInitializer;

class WindowMain
{
    SDL_Window *window;

public:
    WindowMain(SDLInitializer &sdlInitializer);
    void initFrameBuffer();
    SDL_Window *getWindow() { return window; }

    int SCRNWidth = 1280;
    int SCRNHeight = 720;

    SDL_GLContext OpenGLContext;
    SDL_Surface *iconSurface = IMG_Load("./assets/icons/icon.png");

    bool Quit = false;

    unsigned int fbo;
    unsigned int frameBufferTexture;
    unsigned int rbo;
    unsigned int rectVAO, rectVBO;
};