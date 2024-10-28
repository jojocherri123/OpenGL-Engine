#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <glm/mat4x4.hpp>

#include "../glad/glad.h"

class Shader
{
    GLuint id = 0;

    std::string loadShaderAsString(const std::string &filename);
    GLuint compileShader(GLuint type, const std::string &source);
    GLuint createShaderProgram(const std::string &vs, const std::string &fs, const std::string &gs);

public:
    Shader();
    // ~Shader() { deleteShader(); }
    void init(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geomPath = "");

    GLuint getId() const { return id; }
    void use();
    void deleteShader();

    void setFloat(const GLchar *uniform, float value);
    void setInt(const GLchar *uniform, int value);
    void setFloat3(const GLchar *uniform, float value1, float value2, float value3);
    void setFloat3v(const GLchar *uniform, glm::vec3 value1);
    void setFloat4(const GLchar *uniform, float value1, float value2, float value3, float value4);
    void setMatrix4FV(const GLchar *uniform, glm::mat4 value1);
    void set1i(const GLchar *uniform, int value1);
};
