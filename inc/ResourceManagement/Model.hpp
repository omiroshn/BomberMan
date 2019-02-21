#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "ResourceManagement/Mesh.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Texture.hpp"


class Model 
{
public:
    Model(std::string const &path, bool gamma = false);
    void Draw(std::shared_ptr<Shader> shader);
    
private:

    void loadModel(std::string const &path);

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

};

#endif