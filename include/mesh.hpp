#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad.h>

#include "shader.hpp"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    GLuint id;
    string type;
    string path;
};

class Mesh
{
public:
    // mesh Data
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    GLuint VAO;

    // constructor
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

    // render the mesh
    void Draw(const Shader &shader);

private:
    // render data
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};
