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

    ImGui::SeparatorText("Lights");
    ImGui::Text("Color");
    ImGui::ColorEdit4("Light color", (float *)&lightSettings.getLightColor());

    // if (ImGui::Button("Add Point light")){
    //     std::cout << "added a point light" << std::endl;
    //     lightSettings.getPointLightPositions().push_back(glm::vec3(0.0f,0.0f,0.0f));
    // }

    if (selected < lightSettings.getPointLightPositions().size())
    {
        ImGui::Text("Point Light");
        ImGui::PushID(selected);
        ImGui::DragFloat3("", (float *)&lightSettings.getPointLightPositions()[selected], 0.1f);
        ImGui::PopID();
    }
    else if (selected < lightSettings.getPointLightPositions().size() + lightSettings.getSpotLightPositions().size())
    {
        ImGui::Text("Spot Light");
        ImGui::PushID(sizeof(lightSettings.getPointLightPositions()) / sizeof(glm::vec3) + selected);
        ImGui::DragFloat3("Position", (float *)&lightSettings.getSpotLightPositions()[selected - lightSettings.getPointLightPositions().size()], 0.1f);
        ImGui::SliderFloat3("Angle", (float *)&lightSettings.getSpotLightAngles()[selected - lightSettings.getPointLightPositions().size()], -1.0f, 1.0f);
        ImGui::PopID();
    }
    else if (selected < lightSettings.getPointLightPositions().size() + lightSettings.getSpotLightPositions().size() + lightSettings.getDirectionalLightAngles().size())
    {
        ImGui::Text("Directional Light");
        ImGui::PushID(sizeof(lightSettings.getDirectionalLightAngles()) / sizeof(glm::vec3) + sizeof(lightSettings.getPointLightPositions()) / sizeof(glm::vec3) + selected + 1);
        ImGui::DragFloat3("", (float *)&lightSettings.getDirectionalLightAngles()[selected - lightSettings.getPointLightPositions().size() - lightSettings.getSpotLightPositions().size()], 5.0f);
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