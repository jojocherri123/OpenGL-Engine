#include "shader.hpp"

Shader::Shader()
{
}

void Shader::init(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geomPath)
{
    std::string geometryShaderSource;
    if (!geomPath.empty())
    {
        geometryShaderSource = loadShaderAsString(geomPath);
    }

    id = createShaderProgram(loadShaderAsString(vertexPath), loadShaderAsString(fragmentPath), geometryShaderSource);
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::deleteShader()
{
    glDeleteShader(id);
}

void Shader::setFloat(const GLchar *uniform, float value)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniform1f(UNIFORM, value);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

void Shader::setInt(const GLchar *uniform, int value)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniform1i(UNIFORM, value);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

void Shader::setFloat3(const GLchar *uniform, float value1, float value2, float value3)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniform3f(UNIFORM, value1, value2, value3);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

void Shader::setFloat3v(const GLchar *uniform, glm::vec3 value1)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniform3fv(UNIFORM, GL_TRUE, &value1[0]);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

void Shader::setFloat4(const GLchar *uniform, float value1, float value2, float value3, float value4)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniform4f(UNIFORM, value1, value2, value3, value4);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

void Shader::setMatrix4FV(const GLchar *uniform, glm::mat4 value1)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniformMatrix4fv(UNIFORM, 1, GL_FALSE, &value1[0][0]);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

void Shader::set1i(const GLchar *uniform, int value1)
{
    GLint UNIFORM = glGetUniformLocation(id, uniform);
    if (UNIFORM >= 0)
    {
        glUniform1i(UNIFORM, value1);
    }
    else
    {
        std::cout << "error finding " << uniform << " misspell?" << std::endl;
        exit(1);
    }
}

std::string Shader::loadShaderAsString(const std::string &filename)
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

GLuint Shader::compileShader(GLuint type, const std::string &source)
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

GLuint Shader::createShaderProgram(const std::string &vs, const std::string &fs, const std::string &gs)
{
    GLuint programObject = glCreateProgram();

    glAttachShader(programObject, compileShader(GL_VERTEX_SHADER, vs));
    glAttachShader(programObject, compileShader(GL_FRAGMENT_SHADER, fs));
    if (!gs.empty())
    {
        glAttachShader(programObject, compileShader(GL_GEOMETRY_SHADER, gs));
    }

    glLinkProgram(programObject);
    glValidateProgram(programObject);
    return programObject;
}
