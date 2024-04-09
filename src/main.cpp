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
#include <SDL2/SDL_image.h>
#include "includes/glad/glad.h"
#include <GLFW/glfw3.h>
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
glm::vec3 lightPos = glm::vec3(0.0f,0.0f,1.0f);
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
            
            // if (mouseY >= 70 and mouseY <= 270){
            //     mouseY += e.motion.yrel;
            // }else if (mouseY > 270){
            //     mouseY -=1;
            // }else if (mouseY < 70){
            //     mouseY +=1;
            // }

            gCamera.MouseLook(mouseX,mouseY);
        }

    }
        
    const Uint8 *state= SDL_GetKeyboardState(NULL);
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
    
    GLint u_ModelMatrix = glGetUniformLocation(shader.ID,"u_ModelMatrix");
    
    if (u_ModelMatrix>=0){
        glUniformMatrix4fv(u_ModelMatrix,1,GL_FALSE,&model[0][0]);
    }else{
        std::cout << "error finding u_ModelMatrix, misspell?" << std::endl;
        exit(1);
    }

    glm::mat4 view = gCamera.getViewMatrix();

    GLint u_ViewLocation = glGetUniformLocation(shader.ID,"u_ViewMatrix");
    if (u_ViewLocation>=0){
        glUniformMatrix4fv(u_ViewLocation,1,GL_FALSE,&view[0][0]);
    }else{
        std::cout << "error finding u_ViewMatrix, misspell?" << std::endl;
        exit(1);
    }
    

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)SCRNWidth/(float)SCRNHeight,0.1f,100.0f);
    
    GLint u_PerspectionLocation = glGetUniformLocation(shader.ID,"u_Projection");
    if (u_PerspectionLocation>=0){
        glUniformMatrix4fv(u_PerspectionLocation,1,GL_FALSE,&perspective[0][0]);
    }else{
        std::cout << "error finding u_Perspective, misspell?" << std::endl;
        exit(1);
    }

    GLint u_LightColor = glGetUniformLocation(shader.ID,"u_LightColor");
    if (u_LightColor>=0){
        glUniform4f(u_LightColor,lightColor.x,lightColor.y,lightColor.z,lightColor.w);
    }else{
        std::cout << "error finding u_LightColor, misspell?" << std::endl;
        exit(1);
    }

    GLint u_LightPosition = glGetUniformLocation(shader.ID,"u_LightPosition");
    if (u_LightPosition>=0){
        glUniform3f(u_LightPosition,lightPos.x,lightPos.y,lightPos.z);
    }else{
        std::cout << "error finding lightPos, misspell?" << std::endl;
        exit(1);
    }

    GLint u_CamPos = glGetUniformLocation(shader.ID,"u_CamPos");
    if (u_CamPos>=0){
        glUniform3f(u_CamPos,gCamera.mEye.x,gCamera.mEye.y,gCamera.mEye.z);
    }else{
        std::cout << "error finding u_CamPos, misspell?" << std::endl;
        exit(1);
    }


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

    glm::mat4 model = glm::translate(glm::mat4(1.0f),lightPos);

    GLint u_ModelMatrixLight = glGetUniformLocation(Lightshader.ID,"u_ModelMatrixLight");

    if (u_ModelMatrixLight>=0){
        glUniformMatrix4fv(u_ModelMatrixLight,1,GL_FALSE,&model[0][0]);
    }else{
        std::cout << "error finding u_ModelMatrixLight, misspell?" << std::endl;
        exit(1);
    }

    glm::mat4 view = gCamera.getViewMatrix();

    GLint u_ViewLocationLight = glGetUniformLocation(Lightshader.ID,"u_ViewMatrixLight");
    if (u_ViewLocationLight>=0){
        glUniformMatrix4fv(u_ViewLocationLight,1,GL_FALSE,&view[0][0]);
    }else{
        std::cout << "error finding u_ViewMatrixLight, misspell?" << std::endl;
        exit(1);
    }

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)SCRNWidth/(float)SCRNHeight,0.1f,100.0f);
    
    GLint u_PerspectionLocation = glGetUniformLocation(Lightshader.ID,"u_ProjectionLight");
    if (u_PerspectionLocation>=0){
        glUniformMatrix4fv(u_PerspectionLocation,1,GL_FALSE,&perspective[0][0]);
    }else{
        std::cout << "error finding u_ProjectionLight, misspell?" << std::endl;
        exit(1);
    }
    
    GLint u_LightColor = glGetUniformLocation(Lightshader.ID,"u_LightColor");
    if (u_LightColor>=0){
        glUniform4f(u_LightColor,lightColor.x,lightColor.y,lightColor.z,lightColor.w);
    }else{
        std::cout << "error finding u_LightColor, misspell?" << std::endl;
        exit(1);
    }

    LightSource.Draw(Lightshader);
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
    
    backPack.loadModel("../src/content/objects/backpack/backpack.obj");
    LightSource.loadModel("../src/content/objects/light.obj");

    shader.GraphicsPipeLine("../src/includes/shader/shaders/vertex.glsl",
                            "../src/includes/shader/shaders/geometry.glsl",
                            "../src/includes/shader/shaders/fragment.glsl" );

    Lightshader.GraphicsPipeLine("../src/includes/shader/shaders/light.vertex.glsl",
                                "../src/includes/shader/shaders/light.geo.glsl",
                                "../src/includes/shader/shaders/light.fragment.glsl");

    MainLoop();
    CleanUp();
    return 0;
}
