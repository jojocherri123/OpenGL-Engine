#include "app.hpp"
#include "sdlinitializer.hpp"
#include "window.hpp"
#include "openglcontext.hpp"

int main()
{
    SDLInitializer sdlInitializer;
    WindowMain windowMain(sdlInitializer);
    OpenGLContext context(windowMain);
    App app(windowMain, context);
    app.init();
    app.run();
    app.destroy();

    return 0;
}