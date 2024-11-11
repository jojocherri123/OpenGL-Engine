#pragma once

#include <SDL2/SDL.h>

class WindowMain;

class OpenGLContext
{
    SDL_GLContext context;

    void getOpengGLVersionInfo();

public:
    OpenGLContext(WindowMain &windowMain);
};