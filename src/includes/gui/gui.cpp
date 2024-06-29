#include "gui.hpp"


void EngineGUI::setupGUI(SDL_Window *window, SDL_GLContext opengl){

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, opengl);
    ImGui_ImplOpenGL3_Init("#version 460");

}

void EngineGUI::GUISetupDrawWindow(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}


static int selected = 0;


void EngineGUI::GUIconfigWindow(float &fogDensity, 
                                glm::vec3 &fogColor, 
                                glm::vec4 &lightColor, 
                                std::vector<glm::vec3> &pointLightPositions, 
                                std::vector<glm::vec3> &spotLightPositions, 
                                std::vector<glm::vec3> &spotLightAngles, 
                                std::vector<glm::vec3> &directionalLightAngles){
    
    
    ImGui::Begin("Configuration");
    

    ImGui::SeparatorText("General");
        ImGui::Text("Fog");
        ImGui::SliderFloat("Density",(float*)&fogDensity,-1.0f,1.0f);
        ImGui::ColorEdit3("Color",(float*)&fogColor);

    ImGui::SeparatorText("Lights");
        ImGui::Text("Color");
        ImGui::ColorEdit4("Light color",(float*)&lightColor);

        // if (ImGui::Button("Add Point light")){
        //     std::cout << "added a point light" << std::endl;
        //     pointLightPositions.push_back(glm::vec3(0.0f,0.0f,0.0f));
        // }

        if(selected<pointLightPositions.size()){
            ImGui::Text("Point Light");
            ImGui::PushID(selected);
            ImGui::DragFloat3("",(float*)&pointLightPositions[selected],0.1f);
            ImGui::PopID();
        }
        else if(selected<pointLightPositions.size()+spotLightPositions.size()){
            ImGui::Text("Spot Light");
            ImGui::PushID(sizeof(pointLightPositions)/sizeof(glm::vec3)+selected);
            ImGui::DragFloat3("Position",(float*)&spotLightPositions[selected-pointLightPositions.size()],0.1f);
            ImGui::SliderFloat3("Angle",(float*)&spotLightAngles[selected-pointLightPositions.size()],-1.0f,1.0f);
            ImGui::PopID();
        }
        else if (selected<pointLightPositions.size()+spotLightPositions.size()+directionalLightAngles.size()){
            ImGui::Text("Directional Light");
            ImGui::PushID(sizeof(directionalLightAngles)/sizeof(glm::vec3)+sizeof(pointLightPositions)/sizeof(glm::vec3)+selected+1);
            ImGui::DragFloat3("",(float*)&directionalLightAngles[selected-pointLightPositions.size()-spotLightPositions.size()],5.0f);
            ImGui::PopID();
        }

    ImGui::End();
}

void EngineGUI::Objects(glm::vec4 &lightColor, 
                        std::vector<glm::vec3> &pointLightPositions, 
                        std::vector<glm::vec3> &spotLightPositions, 
                        std::vector<glm::vec3> &spotLightAngles, 
                        std::vector<glm::vec3> &directionalLightAngles){

    ImGui::Begin("Objects");

    ImGui::Separator();
    ImGui::BeginChild("pane");
    for (int i = 0; i < pointLightPositions.size() + spotLightPositions.size() + directionalLightAngles.size(); i++)
            {
                char label[128];
                sprintf(label, "Object %d", i);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
    ImGui::EndChild();

    ImGui::End();

    
};

void EngineGUI::GUIrender(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EngineGUI::GUIevent(SDL_Event event){
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void EngineGUI::cleanGUI(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}