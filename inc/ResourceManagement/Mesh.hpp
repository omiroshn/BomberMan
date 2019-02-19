#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Texture.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

// struct Texture {
//     unsigned int id;
//     std::string type;
//     std::string path;
// };

class Mesh
{
public:
    Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<std::shared_ptr<Texture>> aTextures);
    // render the mesh
    void Draw(std::shared_ptr<Shader> shader);

private:
    unsigned int mVBO, mEBO;
    void setupMesh();

private:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<std::shared_ptr<Texture>> mTextures;
    unsigned int mVAO;
};
#endif