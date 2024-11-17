#include <vector>
#include "gui.hpp"
#include "lightsettings.hpp"

void EngineGUI::setup(SDL_Window &window, SDL_GLContext &opengl)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(&window, opengl);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void EngineGUI::setupDrawWindow()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void EngineGUI::createMenus(float &fogDensity,
                            glm::vec3 &fogColor,
                            LightSettings &lightSettings)
{

    static int selected = 0;

    // configuration window

    ImGui::Begin("Configuration");

    ImGui::SeparatorText("General");
    ImGui::Text("Fog");
    ImGui::SliderFloat("Density", (float *)&fogDensity, -1.0f, 1.0f);
    ImGui::ColorEdit3("Color", (float *)&fogColor);

    ImGui::SliderFloat("Shine", (float *)&lightSettings.getShine(), 0.0f, 256.0f);

    ImGui::SeparatorText("Lights");

    ImGui::Checkbox("Show Lights", &lightSettings.getShowLights());

    if (ImGui::Button("Add Point light") && lightSettings.getPointLightPositions().size() < 128)
    {
        std::cout << "added a point light" << std::endl;
        lightSettings.getPointLightPositions().push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        lightSettings.getPointLightColors().push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        lightSettings.getPointLightIntensities().push_back(1.0f);
    }

    if (ImGui::Button("Add Directioanl light") && lightSettings.getDirectionalLightAngles().size() < 16)
    {
        std::cout << "added a directioanl light" << std::endl;
        lightSettings.getDirectionalLightAngles().push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        lightSettings.getDirectionalLightColors().push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        lightSettings.getDirectionalLightIntensities().push_back(1.0f);
    }

    if (ImGui::Button("Add Spot light") && lightSettings.getSpotLightPositions().size() > 0)
    {
        std::cout << "added a spot light" << std::endl;
        lightSettings.getSpotLightPositions().push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        lightSettings.getSpotLightAngles().push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        lightSettings.getSpotLightColors().push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        lightSettings.getSpotLightIntensities().push_back(1.0f);
    }

    if (selected < lightSettings.getPointLightPositions().size())
    {
        ImGui::Text("Point Light");
        ImGui::PushID(selected);
        ImGui::DragFloat3("", (float *)&lightSettings.getPointLightPositions()[selected], 0.1f);
        ImGui::ColorEdit4("Light color", (float *)&lightSettings.getPointLightColors()[selected]);
        ImGui::SliderFloat("Intensity", (float *)&lightSettings.getPointLightIntensities()[selected], 0.0f, 128.0f);
        if (ImGui::Button("Delete Point light") && lightSettings.getSpotLightPositions().size() < 128)
        {
            std::cout << "deleted a point light" << std::endl;
            lightSettings.getPointLightPositions().erase(lightSettings.getPointLightPositions().begin() + selected);
            lightSettings.getPointLightColors().erase(lightSettings.getPointLightColors().begin() + selected);
            lightSettings.getPointLightIntensities().erase(lightSettings.getPointLightIntensities().begin() + selected);
        }
        ImGui::PopID();
    }
    else if (selected < lightSettings.getPointLightPositions().size() + lightSettings.getSpotLightPositions().size())
    {
        ImGui::Text("Spot Light");
        ImGui::PushID(sizeof(lightSettings.getPointLightPositions()) / sizeof(glm::vec3) + selected);
        ImGui::DragFloat3("Position", (float *)&lightSettings.getSpotLightPositions()[selected - lightSettings.getPointLightPositions().size()], 0.1f);
        ImGui::SliderFloat3("Angle", (float *)&lightSettings.getSpotLightAngles()[selected - lightSettings.getPointLightPositions().size()], -1.0f, 1.0f);
        ImGui::ColorEdit4("Light color", (float *)&lightSettings.getSpotLightColors()[selected - lightSettings.getPointLightPositions().size()]);
        ImGui::SliderFloat("Intensity", (float *)&lightSettings.getSpotLightIntensities()[selected - lightSettings.getPointLightPositions().size()], 0.0f, 128.0f);
        if (ImGui::Button("Delete Point light") && lightSettings.getSpotLightPositions().size() > 0)
        {
            std::cout << "deleted a point light" << std::endl;
            lightSettings.getSpotLightPositions().erase(lightSettings.getSpotLightPositions().begin() + (selected - lightSettings.getPointLightPositions().size()));
            lightSettings.getSpotLightAngles().erase(lightSettings.getSpotLightAngles().begin() + (selected - lightSettings.getPointLightPositions().size()));
            lightSettings.getSpotLightColors().erase(lightSettings.getSpotLightColors().begin() + (selected - lightSettings.getPointLightPositions().size()));
            lightSettings.getSpotLightIntensities().erase(lightSettings.getSpotLightIntensities().begin() + (selected - lightSettings.getPointLightPositions().size()));
        }
        ImGui::PopID();
    }
    else if (selected < lightSettings.getPointLightPositions().size() + lightSettings.getSpotLightPositions().size() + lightSettings.getDirectionalLightAngles().size())
    {
        ImGui::Text("Directional Light");
        ImGui::PushID(sizeof(lightSettings.getDirectionalLightAngles()) / sizeof(glm::vec3) + sizeof(lightSettings.getPointLightPositions()) / sizeof(glm::vec3) + selected + 1);
        ImGui::DragFloat3("", (float *)&lightSettings.getDirectionalLightAngles()[selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()], 5.0f);
        ImGui::ColorEdit4("Light color", (float *)&lightSettings.getDirectionalLightColors()[selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()]);
        ImGui::SliderFloat("Intensity", (float *)&lightSettings.getDirectionalLightIntensities()[selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()], 0.0f, 128.0f);
        if (ImGui::Button("Delete Directional light") && lightSettings.getSpotLightPositions().size() > 0)
        {
            std::cout << "deleted a directional light" << std::endl;
            lightSettings.getDirectionalLightAngles().erase(lightSettings.getDirectionalLightAngles().begin() + (selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()));
            lightSettings.getDirectionalLightColors().erase(lightSettings.getDirectionalLightColors().begin() + (selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()));
            lightSettings.getDirectionalLightIntensities().erase(lightSettings.getDirectionalLightIntensities().begin() + (selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()));
        }
        ImGui::PopID();
    }

    ImGui::End();

    // objects window
    ImGui::Begin("Objects");

    ImGui::Separator();
    ImGui::BeginChild("pane");
    for (int i = 0; i < lightSettings.getPointLightPositions().size() + lightSettings.getSpotLightPositions().size() + lightSettings.getDirectionalLightAngles().size(); i++)
    {
        char label[128];
        sprintf(label, "Object %d", i);
        if (ImGui::Selectable(label, selected == i))
            selected = i;
    }
    ImGui::EndChild();

    ImGui::End();
}

void EngineGUI::rightClickMenu()
{

    ImGui::BeginPopup("This is a name");
    ImGui::Text("this is text");
    ImGui::End();
}

void EngineGUI::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EngineGUI::processEvent(SDL_Event &event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void EngineGUI::clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}