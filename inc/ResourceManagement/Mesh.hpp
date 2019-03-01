#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Texture.hpp"
#include "Utilities/AABB.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<std::shared_ptr<Texture>> aTextures);
    void draw(std::shared_ptr<Shader> shader, std::vector<glm::mat4> const & transforms);
    ~Mesh();
private:
    void setupMesh();
    void setInstanceBuffer();
private:
    unsigned int mVBO, mEBO, mVAO, mIBO;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<std::shared_ptr<Texture>> mTextures;
};
#endif