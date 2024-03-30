#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

int SCRNHeight = 420;
int SCRNWidth = 640;

SDL_Window *GraphicsWinow;
SDL_GLContext OpenGLContext;

bool Quit = false;


// VAO
GLuint gVertexArrayObject = 0;
// VBO
GLuint gGLVertexBufferObject = 0;
// IBO
GLuint gIndexBufferObject = 0;

GLuint gPipelineShaderProgram = 0;

float g_uOffset =-2.0f;
float g_uRotate =0;
float g_uScale =0.5f;

Camera gCamera;

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

std::string LoadShaderAsString(const std::string& filename)
{
    std::string result = "";
    std::string line = "";
    
    std::ifstream myFile(filename.c_str());

    // std::cout << myFile.is_open() << std::endl;
    if (myFile.is_open())
    {
        while (std::getline(myFile, line))
        {
            result += line + "\n";
        }
        myFile.close();
    }
    return result;
}

GLuint CompileShader(GLuint type, const std::string &source)
{
    GLuint ShaderObject;

    if (type == GL_VERTEX_SHADER)
    {
        ShaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        ShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char *src = source.c_str();
    glShaderSource(ShaderObject, 1, &src, nullptr);
    glCompileShader(ShaderObject);

    int result;
    glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &length);
        char *errorMessage = new char[length];
        glGetShaderInfoLog(ShaderObject, length, &length, errorMessage);

        if (type == GL_VERTEX_SHADER)
        {
            std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED \n"
                      << errorMessage << std::endl;
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED \n"
                      << errorMessage << std::endl;
        }

        delete[] errorMessage;
        glDeleteShader(ShaderObject);
        return 0;
    }

    return ShaderObject;
}

GLuint CreateShaderProgram(const std::string &vs, const std::string &fs)
{
    GLuint programObject = glCreateProgram();
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);


    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);


    glValidateProgram(programObject);
    return programObject;
}

void GraphicsPipeLine()
{

    std::string VertexShaderSource = LoadShaderAsString("../src/shaders/vertex.glsl");
    std::string FragmentShaderSource = LoadShaderAsString("../src/shaders/fragment.glsl");

    gPipelineShaderProgram = CreateShaderProgram(VertexShaderSource, FragmentShaderSource);
}

void GetOpengGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification()
{

    const std::vector<GLfloat> vertexData{
        // x,   y,    z,     r,    g,    b
        -0.5f,-0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,-0.5f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 1.0f,  1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 1.0f,  0.0f, 0.0f, 1.0f,
    };

    const std::vector<GLuint> indexBufferData{0, 1, 2, 2, 3, 0,
                                              4, 5, 6, 6, 7, 4,
                                              4, 5, 1, 1, 0, 4,
                                              6, 7, 3, 3, 2, 6,
                                              5, 6, 2, 2, 1, 5,
                                              7, 4, 0, 0, 3, 7};

    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1,&gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size()*sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gGLVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gGLVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (GLvoid*)(sizeof(GLfloat)*3));  
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, SCRNWidth, SCRNHeight);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(gPipelineShaderProgram);
    

    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,g_uOffset));
    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::scale(model, glm::vec3(1.0f,g_uScale,1.0f));

    GLint u_ModelMatrix = glGetUniformLocation(gPipelineShaderProgram,"u_ModelMatrix");

    if (u_ModelMatrix>=0){
        glUniformMatrix4fv(u_ModelMatrix,1,GL_FALSE,&model[0][0]);
    }else{
        std::cout << "error finding u_ModelMatrix, misspell?" << std::endl;
        exit(1);
    }

    glm::mat4 view = gCamera.getViewMatrix();

    GLint u_ViewLocation = glGetUniformLocation(gPipelineShaderProgram,"u_ViewMatrix");
    if (u_ViewLocation>=0){
        glUniformMatrix4fv(u_ViewLocation,1,GL_FALSE,&view[0][0]);
    }else{
        std::cout << "error finding u_ViewMatrix, misspell?" << std::endl;
        exit(1);
    }

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)SCRNWidth/(float)SCRNHeight,0.1f,100.0f);
    
    GLint u_PerspectionLocation = glGetUniformLocation(gPipelineShaderProgram,"u_Projection");
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

    // glDrawArrays(GL_TRIANGLES, 0, 4);
    GLCheckError(glDrawElements(GL_TRIANGLES, 36,GL_UNSIGNED_INT,0));
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
    SDL_DestroyWindow(GraphicsWinow);
    SDL_Quit();
}

int main()
{

    InitializeProgram();
    VertexSpecification();
    GraphicsPipeLine();
    MainLoop();
    CleanUp();
    return 0;
}