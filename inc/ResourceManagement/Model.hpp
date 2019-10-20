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

class Animation;
class AABB;
class Model 
{
public:
    Model(std::string const &path, glm::vec3 scale, glm::vec3 offset, glm::vec3 axis, float angle, float glossiness);
    Model& operator=(const Model&) = default;
    ~Model();
    void draw(std::shared_ptr<Shader> const& shader, std::vector<glm::mat4> const& transforms);
    void transform(glm::mat4 const & aTransform);
    void setAnimation(Animation const& anim);

    bool                mAnimated;
private:
    void makeUnitModel();
    void loadModel(std::string const& path);
    void processNode(aiNode const* node,  aiScene const* scene);
    void processMesh(aiMesh const* mesh,  aiScene const* scene);
    std::map<std::string, unsigned int> processBones(aiMesh const* mesh, std::vector<glm::mat4> & aOffsets, std::vector<WeightData> & vertices);

    std::vector<Vertex> loadVertices(aiMesh const* mesh);
    std::vector<unsigned int> loadIndices(aiMesh const* mesh);
    std::vector<std::shared_ptr<Texture>> loadTextures(aiMesh const* mesh, aiScene const* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType texType);
private:
    std::vector<std::unique_ptr<Mesh>> mMeshes;
    std::string         mDirectory;
    AABB                mAABB;
    glm::mat4           mTransFormMatrix;
    float               mGlossiness;
    Assimp::Importer    *mImporter;
};

#endif