#pragma once

#include <vector>

class LightSettings
{

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // must edit numOfPointLight and NR_POINT_LIGHTS (fragment.glsl) if increase or decrease number of coords
    std::vector<glm::vec3> pointLightPositions = {
        glm::vec3(-10.0f, 1.0f, 0.0f),
        glm::vec3(10.0f, 1.0f, 0.0f),
    };

    // must edit numOfSpotLight and NR_SPT_LIGHTS (fragment.glsl) if increase or decrease number of coords
    std::vector<glm::vec3> spotLightPositions = {
        glm::vec3(0.0f, 2.0f, 0.0f),
    };

    std::vector<glm::vec3> spotLightAngles = {
        glm::vec3(0.0f, -1.0f, 0.0f),
    };

    std::vector<glm::vec3> directionalLightAngles = {
        glm::vec3(0.0f, 30.0f, -30.0f),
    };

    unsigned int depthMapFBO;
    unsigned int frameBufferTexture;
    unsigned int rbo;
    unsigned int rectVAO, rectVBO;

public:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    glm::vec4 &getLightColor() { return lightColor; }
    std::vector<glm::vec3> &getPointLightPositions() { return pointLightPositions; }
    std::vector<glm::vec3> &getSpotLightPositions() { return spotLightPositions; }
    std::vector<glm::vec3> &getSpotLightAngles() { return spotLightAngles; }
    std::vector<glm::vec3> &getDirectionalLightAngles() { return directionalLightAngles; }
};