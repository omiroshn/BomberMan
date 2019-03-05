#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "ResourceManagement/Mesh.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Texture.hpp"
#include "Utilities/AABB.hpp"


class AABB;
class Model 
{
public:
    Model(std::string const &path, bool gamma = false);
    ~Model();
    void draw(std::shared_ptr<Shader> shader, std::vector<glm::mat4> & transforms);
    AABB getAABB() const;
    void transform(glm::mat4 const & aTransform);

private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    void processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
private:
    std::vector<std::unique_ptr<Mesh>> mMeshes;
    std::string mDirectory;
    bool mGammaCorrection;
    AABB mAABB;
    glm::mat4 mTransFormMatrix;
};

#endif