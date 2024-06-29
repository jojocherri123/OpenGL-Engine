#ifndef GUI_HPP
#define GUI_HPP

#include <iostream>
#include <glm/glm.hpp>
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <vector>

class EngineGUI{

    public:
        
        void setupGUI(SDL_Window *window, SDL_GLContext opengl);

        void GUIevent(SDL_Event event);
        void GUISetupDrawWindow();
        void GUIconfigWindow(float &fogDensity, 
                            glm::vec3 &fogColor, 
                            glm::vec4 &lightColor, 
                            std::vector<glm::vec3> &pointLightPositions, 
                            std::vector<glm::vec3> &spotLightPositions, 
                            std::vector<glm::vec3> &spotLightAngles, 
                            std::vector<glm::vec3> &directionalLightAngles);

        void Objects(glm::vec4 &lightColor, 
                        std::vector<glm::vec3> &pointLightPositions, 
                        std::vector<glm::vec3> &spotLightPositions, 
                        std::vector<glm::vec3> &spotLightAngles, 
                        std::vector<glm::vec3> &directionalLightAngles);
                        
        void GUIrender();

        void cleanGUI();


};

#endif