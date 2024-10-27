#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class WindowMain
{
public:
    int SCRNWidth = 1280;
    int SCRNHeight = 720;

    SDL_Window *GraphicsWinow;
    SDL_GLContext OpenGLContext;
    SDL_Surface *iconSurface = IMG_Load("./assets/icons/icon.png");

    bool Quit = false;

    unsigned int fbo;
    unsigned int frameBufferTexture;
    unsigned int rbo;
    unsigned int rectVAO, rectVBO;
};