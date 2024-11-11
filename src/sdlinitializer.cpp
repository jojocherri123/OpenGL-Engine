#include <SDL2/SDL.h>

#include "sdlinitializer.hpp"
#include "errorhandler.hpp"

SDLInitializer::SDLInitializer()
{
    executeWithHandler(SDL_Init, SDL_INIT_VIDEO);

    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_CONTEXT_MINOR_VERSION, 6);
    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_DOUBLEBUFFER, 1);
    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_DEPTH_SIZE, 24);

    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_MULTISAMPLEBUFFERS, 1);
    executeWithHandler(SDL_GL_SetAttribute, SDL_GL_MULTISAMPLESAMPLES, 4);
}