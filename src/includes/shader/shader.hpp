#ifndef SHADER_HPP
#define SHADER_HPP

#include<string>
#include <fstream>
#include<iostream>

#include "../glad/glad.h"

class Shader{

    public:

        GLuint ID = 0;

        void GraphicsPipeLine(const char* vertexPath, const char* geomPath, const char* fragmentPath)
        {

            std::string VertexShaderSource = LoadShaderAsString(vertexPath);
            std::string GeometryShaderSource = LoadShaderAsString(geomPath);
            std::string FragmentShaderSource = LoadShaderAsString(fragmentPath);

            ID = CreateShaderProgram(VertexShaderSource,GeometryShaderSource,FragmentShaderSource);
                
        }

        void use(){
            glUseProgram(ID);
        }

        void deleteShader(){
            glDeleteShader(ID);
        }

    private:

        std::string LoadShaderAsString(const std::string& filename)
        {
            std::string result = "";
            std::string line = "";
            
            std::ifstream myFile(filename.c_str());

            if (myFile.is_open())
            {
                while (std::getline(myFile, line))
                {
                    result += line + "\n";
                }
                myFile.close();
            }
            return result;
        };

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
            else if (type == GL_GEOMETRY_SHADER)
            {
                ShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
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
                    std::cerr << "ERROR: VERTEX SHADER COMPILATION FAILED \n"
                            << errorMessage << std::endl;
                }
                else if (type == GL_FRAGMENT_SHADER)
                {
                    std::cerr << "ERROR: FRAGMENT SHADER COMPILATION FAILED \n"
                            << errorMessage << std::endl;
                }
                else if (type == GL_GEOMETRY_SHADER)
                {
                    std::cerr << "ERROR: GEOMETRY SHADER COMPILATION FAILED \n"
                            << errorMessage << std::endl;
                }

                delete[] errorMessage;
                glDeleteShader(ShaderObject);
                return 0;
            }

            return ShaderObject;
        }

        GLuint CreateShaderProgram(const std::string &vs, const std::string &gs,  const std::string &fs)
        {
            GLuint programObject = glCreateProgram();
            GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vs);
            GLuint myGeometryShader = CompileShader(GL_GEOMETRY_SHADER, gs);
            GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

            glAttachShader(programObject, myVertexShader);
            glAttachShader(programObject, myGeometryShader);
            glAttachShader(programObject, myFragmentShader);

            
            glLinkProgram(programObject);

            glValidateProgram(programObject);
            return programObject;
        }

};

#endif