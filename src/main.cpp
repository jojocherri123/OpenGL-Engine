#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <sstream>

#include "includes/imgui/imgui.h"
#include "includes/imgui/backends/imgui_impl_sdl2.h"
#include "includes/imgui/backends/imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "includes/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "includes/camera/camera.hpp"
#include "includes/importModel/model.hpp"
#include "includes/shader/shader.hpp"
#include "includes/gui/gui.hpp"
#include "includes/window/window.hpp"

class LightSettings{
    
    public:
        glm::vec4 lightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);

    //must edit numOfPointLight and NR_POINT_LIGHTS (fragment.glsl) if increase or decrease number of coords
        std::vector<glm::vec3> pointLightPositions = {
            glm::vec3(-10.0f, 1.0f, 0.0f),
            glm::vec3(10.0f,1.0f,0.0f)
        };

    //must edit numOfSpotLight and NR_SPT_LIGHTS (fragment.glsl) if increase or decrease number of coords
        std::vector<glm::vec3> SpotLightPositions = {
            glm::vec3(0.0f, 2.0f, 0.0f),
            
        };
        std::vector<glm::vec3> SpotLightAngles = {
            glm::vec3(0.0f, -1.0f, 0.0f),
            
        };

        std::vector<glm::vec3> directionalLightAngles = {
            glm::vec3(0.0f, 100.0f, -100.0f),
        };
};
LightSettings lightSettings;

WindowMain windowMain;
Camera gCamera;
GLuint gtexture;
Shader shader;
Model backPack;
EngineGUI engineGui;
Shader Lightshader;
Model LightSource;
Shader frameBufferShader;

float FogDensity;
glm::vec3 FogColor;


static void GLclearAllErrors(){
    while(glGetError() != GL_NO_ERROR){
    }
}

static bool GLcheckErrorStatus(const char* function, int line){
    while(GLenum error = glGetError()){
        std::cout << "OpenGL ERROR: "<< error 
                    << "\tLine: " << line 
                    << "\tFunction: "<< function << std::endl;

        return true;
    }
    return false;
}

#define GLCheckError(x) GLclearAllErrors(); x ;GLcheckErrorStatus(#x,__LINE__);

void GetOpengGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitializeProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL COUDLNT LOAD" << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

   windowMain.GraphicsWinow = SDL_CreateWindow("Shade Line Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                                windowMain.SCRNWidth,windowMain.SCRNHeight,
                                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (windowMain.GraphicsWinow == nullptr)
    {
        std::cout << "Couldn't create window" << std::endl;
        exit(1);
    }

   windowMain.OpenGLContext = SDL_GL_CreateContext(windowMain.GraphicsWinow);
    if (windowMain.OpenGLContext == nullptr)
    {
        std::cout << "Couldn't create opengl context" << std::endl;
        exit(1);
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Couldn't initialize glad" << std::endl;
        exit(1);
    }

    SDL_SetWindowIcon(windowMain.GraphicsWinow,windowMain.iconSurface);

    GetOpengGLVersionInfo();
}


void Input()
{
    static int mouseX =windowMain.SCRNWidth/2;
    static int mouseY =windowMain.SCRNHeight/2;
    static bool inputCapture = false;
    

    const Uint8 *state= SDL_GetKeyboardState(NULL);
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        engineGui.GUIevent(e);
        
        if (e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_m && inputCapture == false){
                inputCapture=true;
                SDL_SetRelativeMouseMode(SDL_TRUE);

            }else if (e.key.keysym.sym == SDLK_m && inputCapture == true){
                inputCapture=false;
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
        }


        if (e.type == SDL_QUIT)
        {
            std::cout << "Exited Succesfully" << std::endl;
            windowMain.Quit = true;
        }


        if(e.type == SDL_MOUSEMOTION && inputCapture==true){
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gCamera.MouseLook(mouseX,mouseY);
        }

    }
        
    if (state[SDL_SCANCODE_ESCAPE]){
        std::cout << "Exited Succesfully" << std::endl;
        windowMain.Quit = true;
    }

    if (inputCapture){
        if (state[SDL_SCANCODE_W]){
            gCamera.MoveForward(0.1f);
        }
        if (state[SDL_SCANCODE_S]){
            gCamera.MoveBackward(0.1f);
        }

        if (state[SDL_SCANCODE_D]){
            gCamera.MoveLeft(0.1f);
        }
        if (state[SDL_SCANCODE_A]){
            gCamera.MoveRight(0.1f);
        }
        if (state[SDL_SCANCODE_E]){
            gCamera.MoveUp(0.1f);
        }
        if (state[SDL_SCANCODE_Q]){
            gCamera.MoveDown(0.1f);
        }
    }
}


void PreDraw()
{

    

    glViewport(0, 0, windowMain.SCRNWidth, windowMain.SCRNHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, windowMain.fbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_FRAMEBUFFER_SRGB); 

    shader.use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
    // model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f,0.0f,0.0f));
    model = glm::scale(model, glm::vec3(2.0f,2.0f,2.0f));
    

    shader.SetMatrix4FV("u_ModelMatrix", model);

    glm::mat4 view = gCamera.getViewMatrix();
    shader.SetMatrix4FV("u_ViewMatrix", view);

    glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)windowMain.SCRNWidth/(float)windowMain.SCRNHeight,0.1f,1000.0f);
    shader.SetMatrix4FV("u_Projection", perspective);

    shader.SetFloat4("u_LightColor",lightSettings.lightColor.x, lightSettings.lightColor.y, lightSettings.lightColor.z, lightSettings.lightColor.w);

    shader.SetFloat3v("pointLights[0].position", lightSettings.pointLightPositions[0]);
    shader.SetFloat3v("pointLights[1].position", lightSettings.pointLightPositions[1]);
    shader.SetFloat3v("directionalLight[0].angle", lightSettings.directionalLightAngles[0]);
    shader.SetFloat3v("spotLight[0].position", lightSettings.SpotLightPositions[0]);
    shader.SetFloat3v("spotLight[0].angle", lightSettings.SpotLightAngles[0]);
    shader.SetFloat("FogDensity",FogDensity);
    shader.SetFloat3v("FogColor",FogColor);

    shader.SetFloat3("u_CamPos",gCamera.mEye.x,gCamera.mEye.y,gCamera.mEye.z);

    shader.SetFloat("material.shininess", 8.0f);

    
}

void Draw()
{   

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    backPack.Draw(shader);
    glUseProgram(0);
}

void Light(){

    Lightshader.use();

    int numOfPointLight = lightSettings.pointLightPositions.size();
    int numOfSpotLight = lightSettings.SpotLightPositions.size();
    // shader.SetInt("numOfPointLight", numOfPointLight);
    // shader.SetInt("NR_POINT_LIGHTS", numOfPointLight);
    // shader.SetInt("NR_SPT_LIGHTS", numOfSpotLight);

    for (int i=0; i< numOfPointLight;i++){

        glm::mat4 model = glm::translate(glm::mat4(1.0f),lightSettings.pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));

        Lightshader.SetMatrix4FV("u_ModelMatrixLight", model);

        glm::mat4 view = gCamera.getViewMatrix();
        Lightshader.SetMatrix4FV("u_ViewMatrixLight", view);

        glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)windowMain.SCRNWidth/(float)windowMain.SCRNHeight,0.1f,1000.0f);
        Lightshader.SetMatrix4FV("u_ProjectionLight", perspective);
        
        Lightshader.SetFloat4("u_LightColor", lightSettings.lightColor.x, lightSettings.lightColor.y, lightSettings.lightColor.z, lightSettings.lightColor.w);
        
        LightSource.Draw(Lightshader);
    };

    for (int i=0; i< numOfSpotLight;i++){

        glm::mat4 model = glm::translate(glm::mat4(1.0f), lightSettings.SpotLightPositions[i]);
        model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));

        Lightshader.SetMatrix4FV("u_ModelMatrixLight", model);

        glm::mat4 view = gCamera.getViewMatrix();
        Lightshader.SetMatrix4FV("u_ViewMatrixLight", view);

        glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)windowMain.SCRNWidth/(float)windowMain.SCRNHeight,0.1f,1000.0f);
        Lightshader.SetMatrix4FV("u_ProjectionLight", perspective);
        
        Lightshader.SetFloat4("u_LightColor", lightSettings.lightColor.x, lightSettings.lightColor.y, lightSettings.lightColor.z, lightSettings.lightColor.w);
        
        LightSource.Draw(Lightshader);
    };
}

void MainLoop(){
    

    SDL_WarpMouseInWindow(windowMain.GraphicsWinow, windowMain.SCRNWidth/2, windowMain.SCRNHeight/2);
    while (!windowMain.Quit)
    {
        
        Input();
        PreDraw();

        engineGui.GUISetupDrawWindow();

        Draw();
        Light();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle

        static float gamma = 2.2f;

		frameBufferShader.use();
        frameBufferShader.Set1i("screenTexture",0);
        frameBufferShader.SetFloat("gamma",gamma);
		glBindVertexArray(windowMain.rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, windowMain.frameBufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::ShowDemoWindow();

        engineGui.GUIwindows(FogDensity, 
                                FogColor, 
                                lightSettings.lightColor, 
                                lightSettings.pointLightPositions, 
                                lightSettings.SpotLightPositions, 
                                lightSettings.SpotLightAngles, 
                                lightSettings.directionalLightAngles
                                );


        engineGui.GUIrender();

        SDL_GL_SwapWindow(windowMain.GraphicsWinow);
    }
}

void CleanUp()
{   
    engineGui.cleanGUI();
    shader.deleteShader();
    Lightshader.deleteShader();
    frameBufferShader.deleteShader();
    SDL_DestroyWindow(windowMain.GraphicsWinow);
    SDL_Quit();
}

void frameBuffer(){

    float rectangleVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &windowMain.rectVAO);
	glGenBuffers(1, &windowMain.rectVBO);
	glBindVertexArray(windowMain.rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, windowMain.rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glGenFramebuffers(1, &windowMain.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,windowMain.fbo);

    glGenTextures(1, &windowMain.frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, windowMain.frameBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowMain.SCRNWidth, windowMain.SCRNHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, windowMain.frameBufferTexture, 0);
    
    glGenRenderbuffers(1, &windowMain.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, windowMain.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowMain.SCRNWidth, windowMain.SCRNHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, windowMain.rbo);

    auto fboStatus =glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: "<< fboStatus << endl;

}

int main()
{
    
    InitializeProgram();


    frameBuffer();
    // stbi_set_flip_vertically_on_load(true);
    
    backPack.loadModel("./src/content/objects/sponza-scene/source/sponza/sponza.obj");
    LightSource.loadModel("./src/content/objects/light.obj");

    shader.GraphicsPipeLine("./src/includes/shader/shaders/vertex.glsl",
                            "./src/includes/shader/shaders/geometry.glsl",
                            "./src/includes/shader/shaders/fragment.glsl" );

    Lightshader.GraphicsPipeLine("./src/includes/shader/shaders/light.vertex.glsl",
                                "./src/includes/shader/shaders/light.geo.glsl",
                                "./src/includes/shader/shaders/light.fragment.glsl");

    frameBufferShader.GraphicsPipeLine("./src/includes/shader/shaders/fbo.vertex.glsl",
                                "./src/includes/shader/shaders/fbo.geo.glsl",
                                "./src/includes/shader/shaders/fbo.fragment.glsl");

    engineGui.setupGUI(windowMain.GraphicsWinow, windowMain.OpenGLContext);
    
    MainLoop();
    CleanUp();
    return 0;
}