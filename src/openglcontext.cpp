#include <glad.h>

#include "openglcontext.hpp"
#include "errorhandler.hpp"
#include "window.hpp"

OpenGLContext::OpenGLContext(WindowMain &windowMain)
{
    windowMain.OpenGLContext = executeWithNullHandler(SDL_GL_CreateContext, windowMain.getWindow());

    executeWithHandler([](GLADloadproc x)
                       { return !gladLoadGLLoader(x); }, SDL_GL_GetProcAddress);

    getOpengGLVersionInfo();
}

void OpenGLContext::getOpengGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

