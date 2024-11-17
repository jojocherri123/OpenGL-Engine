#pragma once

#include <vector>

class LightSettings
{

    bool showLights = true;

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float shininess = 32;

    std::vector<glm::vec3> pointLightPositions = {
        glm::vec3(-10.0f, 1.0f, 0.0f),
        glm::vec3(10.0f, 1.0f, 0.0f),
    };
    std::vector<glm::vec4> pointLightColors = {
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    };
    std::vector<float> pointLightIntensities = {1.0f,1.0f};

    std::vector<glm::vec3> spotLightPositions = {
        glm::vec3(0.0f, 2.0f, 0.0f),
    };
    std::vector<glm::vec4> spotLightColors = {
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    };
    std::vector<glm::vec3> spotLightAngles = {
        glm::vec3(0.0f, -1.0f, 0.0f),
    };
    std::vector<float> spotLightIntensities = {1.0f};

    std::vector<glm::vec3> directionalLightAngles = {
        glm::vec3(0.0f, 30.0f, -30.0f),
    };
    std::vector<glm::vec4> directionalLightColors = {
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    };
    std::vector<float> directionalLightIntensities = {1.0f};

    unsigned int depthMapFBO;
    unsigned int frameBufferTexture;
    unsigned int rbo;
    unsigned int rectVAO, rectVBO;

public:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    bool &getShowLights() {return showLights;}
    glm::vec4 &getLightColor() { return lightColor; }
    float &getShine() {return shininess;}
    std::vector<glm::vec3> &getPointLightPositions() { return pointLightPositions; }
    std::vector<glm::vec4> &getPointLightColors() { return pointLightColors; }
    std::vector<float> &getPointLightIntensities() { return pointLightIntensities; }

    std::vector<glm::vec3> &getSpotLightPositions() { return spotLightPositions; }
    std::vector<glm::vec3> &getSpotLightAngles() { return spotLightAngles; }
    std::vector<glm::vec4> &getSpotLightColors() { return spotLightColors; }
    std::vector<float> &getSpotLightIntensities() { return spotLightIntensities; }

    std::vector<glm::vec3> &getDirectionalLightAngles() { return directionalLightAngles; }
    std::vector<glm::vec4> &getDirectionalLightColors() { return directionalLightColors; }
    std::vector<float> &getDirectionalLightIntensities() { return directionalLightIntensities; }
};