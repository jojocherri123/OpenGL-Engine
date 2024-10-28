#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "lightsettings.hpp"

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

void Shader::apply(Camera &camera, LightSettings &lightSettings, float fogDensity, glm::vec3 &fogColor, float aspectRatio)
{
    use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f,0.0f,0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

    setMatrix4FV("u_ModelMatrix", model);

    glm::mat4 view = camera.getViewMatrix();
    setMatrix4FV("u_ViewMatrix", view);

    glm::mat4 perspective = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 1000.0f);
    setMatrix4FV("u_Projection", perspective);

    setFloat4("u_LightColor", lightSettings.getLightColor().x, lightSettings.getLightColor().y, lightSettings.getLightColor().z, lightSettings.getLightColor().w);

    setFloat3v("pointLights[0].position", lightSettings.getPointLightPositions()[0]);
    setFloat3v("pointLights[1].position", lightSettings.getPointLightPositions()[1]);
    setFloat3v("directionalLight[0].angle", lightSettings.getDirectionalLightAngles()[0]);
    setFloat3v("spotLight[0].position", lightSettings.getSpotLightPositions()[0]);
    setFloat3v("spotLight[0].angle", lightSettings.getSpotLightAngles()[0]);
    setFloat("FogDensity", fogDensity);
    setFloat3v("FogColor", fogColor);

    setFloat3("u_CamPos", camera.mEye.x, camera.mEye.y, camera.mEye.z);

    setFloat("material.shininess", 8.0f);
}

void Shader::applyLight(Camera &camera, LightSettings &lightSettings, glm::vec3 &lightPosition, float aspectRatio)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPosition);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    setMatrix4FV("u_ModelMatrixLight", model);

    glm::mat4 view = camera.getViewMatrix();
    setMatrix4FV("u_ViewMatrixLight", view);

    glm::mat4 perspective = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 1000.0f);
    setMatrix4FV("u_ProjectionLight", perspective);

    setFloat4("u_LightColor", lightSettings.getLightColor().x, lightSettings.getLightColor().y, lightSettings.getLightColor().z, lightSettings.getLightColor().w);
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
