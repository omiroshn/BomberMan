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

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool gamma = false);
    // draws the model, and thus all its meshes
    void Draw(std::shared_ptr<Shader> shader);
    
private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

};

#endif