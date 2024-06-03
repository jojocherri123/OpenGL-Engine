#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <sstream>

#include <SDL2/SDL.h>
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


int SCRNHeight = 800;
int SCRNWidth = 800;

SDL_Window *GraphicsWinow;
SDL_GLContext OpenGLContext;

bool Quit = false;


// VAO
GLuint gVertexArrayObject = 0;
// VBO
GLuint gGLVertexBufferObject = 0;
// IBO
GLuint gIndexBufferObject = 0;


float g_uOffset =-2.0f;
float g_uRotate =0;
float g_uScale =1.0f;

Camera gCamera;

GLuint gtexture;

Shader shader;
Model backPack;


glm::vec4 lightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);

//must edit numOfPointLight if increase or decrease number of coords
glm::vec3 pointLightPositions[] = {
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(-3.0f,-3.0f,-3.0f)
    };
glm::vec3 directionalLightAngles[] = {
        glm::vec3(0.0f, 10.0f, -10.0f),
    };
//must edit numOfSpotLight if increase or decrease number of coords
glm::vec3 SpotLightPositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        
    };

Shader Lightshader;
Model LightSource;


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

    GraphicsWinow = SDL_CreateWindow("Opengl 3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                     SCRNWidth, SCRNHeight,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (GraphicsWinow == nullptr)
    {
        std::cout << "Couldn't create window" << std::endl;
        exit(1);
    }

    OpenGLContext = SDL_GL_CreateContext(GraphicsWinow);
    if (OpenGLContext == nullptr)
    {
        std::cout << "Couldn't create opengl context" << std::endl;
        exit(1);
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Couldn't initialize glad" << std::endl;
        exit(1);
    }

    GetOpengGLVersionInfo();
}


void Input()
{
    static int mouseX = SCRNWidth/2;
    static int mouseY = SCRNHeight/2;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "Exited Succesfully" << std::endl;
            Quit = true;
        }
        
        if(e.type == SDL_MOUSEMOTION){
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gCamera.MouseLook(mouseX,mouseY);
        }

    }
        
    const Uint8 *state= SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]){
        std::cout << "Exited Succesfully" << std::endl;
        Quit = true;
    }
    
    if (state[SDL_SCANCODE_W]){
        // g_uOffset+=0.01f;
        gCamera.MoveForward(0.1f);
    }
    if (state[SDL_SCANCODE_S]){
        // g_uOffset-=0.01f;
        gCamera.MoveBackward(0.1f);
    }

    if (state[SDL_SCANCODE_D]){
        gCamera.MoveLeft(0.1f);
    }
    if (state[SDL_SCANCODE_A]){
        gCamera.MoveRight(0.1f);
    }

    if (state[SDL_SCANCODE_E]){
        g_uScale-=0.05f;
    }
    if (state[SDL_SCANCODE_Q]){
        g_uScale+=0.05f;
    }
}

void PreDraw()
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, SCRNWidth, SCRNHeight);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    
    shader.use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,g_uOffset));
    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::scale(model, glm::vec3(1.0f,g_uScale,1.0f));
    

    shader.SetMatrix4FV("u_ModelMatrix", model);

    glm::mat4 view = gCamera.getViewMatrix();
    shader.SetMatrix4FV("u_ViewMatrix", view);

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)SCRNWidth/(float)SCRNHeight,0.1f,100.0f);
    shader.SetMatrix4FV("u_Projection", perspective);

    shader.SetFloat4("u_LightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);

    // shader.SetFloat3("u_LightPosition",lightPos.x,lightPos.y,lightPos.z);
    shader.SetFloat3v("pointLights[0].position",pointLightPositions[0]);
    shader.SetFloat3v("pointLights[1].position",pointLightPositions[1]);
    shader.SetFloat3v("directionalLight[0].angle",directionalLightAngles[0]);
    shader.SetFloat3v("spotLight[0].position",SpotLightPositions[0]);
    shader.SetFloat3("spotLight[0].angle",0.0f,0.0f,-1.0f);

    shader.SetFloat3("u_CamPos",gCamera.mEye.x,gCamera.mEye.y,gCamera.mEye.z);

    shader.SetFloat("material.shininess", 32.0f);

}

void Draw()
{   

    glBindVertexArray(gVertexArrayObject);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    backPack.Draw(shader);

    glUseProgram(0);
}

void Light(){

    Lightshader.use();

    int numOfPointLight = 2;
    int numOfSpotLight = 1;
    // shader.SetInt("numOfPointLight", numOfPointLight);

    for (int i=0; i< numOfPointLight;i++){

        glm::mat4 model = glm::translate(glm::mat4(1.0f),pointLightPositions[i]);

        Lightshader.SetMatrix4FV("u_ModelMatrixLight", model);

        glm::mat4 view = gCamera.getViewMatrix();
        Lightshader.SetMatrix4FV("u_ViewMatrixLight", view);

        glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)SCRNWidth/(float)SCRNHeight,0.1f,100.0f);
        Lightshader.SetMatrix4FV("u_ProjectionLight", perspective);
        
        Lightshader.SetFloat4("u_LightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);
        
        LightSource.Draw(Lightshader);
    };

    for (int i=0; i< numOfSpotLight;i++){

        glm::mat4 model = glm::translate(glm::mat4(1.0f),SpotLightPositions[i]);

        Lightshader.SetMatrix4FV("u_ModelMatrixLight", model);

        glm::mat4 view = gCamera.getViewMatrix();
        Lightshader.SetMatrix4FV("u_ViewMatrixLight", view);

        glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)SCRNWidth/(float)SCRNHeight,0.1f,100.0f);
        Lightshader.SetMatrix4FV("u_ProjectionLight", perspective);
        
        Lightshader.SetFloat4("u_LightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);
        
        LightSource.Draw(Lightshader);
    };
}

void MainLoop()
{   
    SDL_WarpMouseInWindow(GraphicsWinow, SCRNWidth/2, SCRNHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    while (!Quit)
    {
        Input();

        PreDraw();

        Draw();
        Light();

        SDL_GL_SwapWindow(GraphicsWinow);
    }
}

void CleanUp()
{
    shader.deleteShader();
    Lightshader.deleteShader();
    SDL_DestroyWindow(GraphicsWinow);
    SDL_Quit();
}

int main()
{
    
    InitializeProgram();
    stbi_set_flip_vertically_on_load(true);
    
    backPack.loadModel("./src/content/objects/backpack/backpack.obj");
    LightSource.loadModel("./src/content/objects/light.obj");

    shader.GraphicsPipeLine("./src/includes/shader/shaders/vertex.glsl",
                            "./src/includes/shader/shaders/geometry.glsl",
                            "./src/includes/shader/shaders/fragment.glsl" );

    Lightshader.GraphicsPipeLine("./src/includes/shader/shaders/light.vertex.glsl",
                                "./src/includes/shader/shaders/light.geo.glsl",
                                "./src/includes/shader/shaders/light.fragment.glsl");

    MainLoop();
    CleanUp();
    return 0;
}