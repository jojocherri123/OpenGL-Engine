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
Model ourModel;


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

void DrawCube()
{

    std::vector<GLfloat> VertexData{
        // x,   y,    z,    X, Y textures
        -0.5,-0.5, 0.5, 0.0, 4.0,
         0.5,-0.5, 0.5, 4.0, 4.0,
         0.5, 0.5, 0.5, 4.0, 0.0,
        -0.5, 0.5, 0.5, 0.0, 0.0,
 
        -0.5,-0.5,-0.5, 0.0, 4.0,
         0.5,-0.5,-0.5, 4.0, 4.0,
         0.5, 0.5,-0.5, 4.0, 0.0,
        -0.5, 0.5,-0.5, 0.0, 0.0,
 
         0.5,-0.5,-0.5, 0.0, 4.0,
         0.5, 0.5,-0.5, 4.0, 4.0,
         0.5, 0.5, 0.5, 4.0, 0.0,
         0.5,-0.5, 0.5, 0.0, 0.0,
 
        -0.5, 0.5,-0.5, 0.0, 4.0,
        -0.5,-0.5,-0.5, 4.0, 4.0,
        -0.5,-0.5, 0.5, 4.0, 0.0,
        -0.5, 0.5, 0.5, 0.0, 0.0,
 
        -0.5,-0.5,-0.5, 0.0, 4.0,
         0.5,-0.5,-0.5, 4.0, 4.0,
         0.5,-0.5, 0.5, 4.0, 0.0,
        -0.5,-0.5, 0.5, 0.0, 0.0,
 
         0.5, 0.5,-0.5, 0.0, 4.0,
        -0.5, 0.5,-0.5, 4.0, 4.0,
        -0.5, 0.5, 0.5, 4.0, 0.0,
         0.5, 0.5, 0.5, 0.0, 0.0

    };

        std::vector<GLuint> IndexBufferData{ 0,  1,  2,  2,  3, 0,
                                             4,  5,  6,  6,  7, 4,
                                             8,  9, 10, 10, 11, 8,
                                            12, 13, 14, 14, 15, 12,
                                            16, 17, 18, 18, 19, 16,
                                            20, 21, 22, 22, 23, 20 };


    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);
    glGenBuffers(1,&gIndexBufferObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferData.size()*sizeof(GLuint), IndexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gGLVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gGLVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(GLfloat), VertexData.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (GLvoid*)0);

    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (GLvoid*)(sizeof(GLfloat)*3));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (GLvoid*)(sizeof(GLfloat)*3));
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    // glDisableVertexAttribArray(2);
    

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

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


}

void Draw()
{   
    
    glBindVertexArray(gVertexArrayObject);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    ourModel.Draw(shader);


    glUseProgram(0);
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

        SDL_GL_SwapWindow(GraphicsWinow);
    }
}

void CleanUp()
{
    shader.deleteShader();
    SDL_DestroyWindow(GraphicsWinow);
    SDL_Quit();
}

int main()
{
    
    InitializeProgram();
    // stbi_set_flip_vertically_on_load(true);

    ourModel.loadModel("../src/content/objects/monkey.obj");
    shader.GraphicsPipeLine("../src/includes/shader/shaders/vertex.glsl","../src/includes/shader/shaders/fragment.glsl");

    MainLoop();
    CleanUp();
    return 0;
}