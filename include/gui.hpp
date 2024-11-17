#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_opengl3.h"

class LightSettings;

class EngineGUI
{

public:
    void setup(SDL_Window &window, SDL_GLContext &opengl);
    void processEvent(SDL_Event &event);
    void setupDrawWindow();
    void createMenus(float &fogDensity,
                     glm::vec3 &fogColor,
                     LightSettings &lightSettings);

    void rightClickMenu();

    void render();
    void clean();
};
