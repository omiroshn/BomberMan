#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "CustomException.hpp"

namespace
{
    glm::mat4			aiMatToGlmMat(aiMatrix4x4 ai)
    {
        glm::mat4 mat;
        mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
        mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
        mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
        mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
        return mat;
    }
}

Model::Model(std::string const &path) : mTransFormMatrix(glm::mat4(1.0f)), mImporter(new Assimp::Importer())
{
    loadModel(path);
}

Model::~Model()
{
    delete mImporter;
}

void Model::loadModel(std::string const &path)
{
    const aiScene* scene = mImporter->ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw CustomException(std::string("ASSIMP::") + mImporter->GetErrorString());
    mDirectory = path.substr(0, path.find_last_of('/'));
    mAnimated = scene->HasAnimations();
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode const* node, aiScene const* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh const* mesh,  aiScene const* scene)
{
    std::vector<Vertex> vertices(loadVertices(mesh));
    std::vector<unsigned int> indices(loadIndices(mesh));
    std::vector<std::shared_ptr<Texture>> textures(loadTextures(mesh, scene));

    mMeshes.emplace_back(std::make_unique<Mesh>(vertices, indices, textures, scene, mesh));
}

std::vector<Vertex> Model::loadVertices(aiMesh const* mesh)
{
    std::vector<Vertex> vertices{};

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;
        if(mesh->mTextureCoords[0])
        {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        mAABB += vertex.Position;
    }
    return std::move(vertices);
}

std::vector<unsigned int> Model::loadIndices(aiMesh const* mesh)
{
    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    return std::move(indices);
}

std::vector<std::shared_ptr<Texture>> Model::loadTextures(aiMesh const* mesh, aiScene const* scene)
{
    std::vector<std::shared_ptr<Texture>> textures;

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<std::shared_ptr<Texture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return std::move(textures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texFullPath(mDirectory + "/" + std::string(str.C_Str()));
        auto texture = RESOURCES.loadTextureFromFile(texFullPath.c_str(), typeName, true);
        textures.push_back(texture);
    }
    return textures;
}


void Model::draw(std::shared_ptr<Shader> shader, std::vector<glm::mat4> & transforms)
{
    for (auto &transform : transforms)
    {
        transform *= mTransFormMatrix;
    }

    for(auto &mesh : mMeshes)
    {
        if (mAnimated)
            mesh->doAnimation();
        mesh->draw(shader, transforms);
    }
}

AABB Model::getAABB() const
{
    return mAABB;
}

void Model::transform(glm::mat4 const & aTransform)
{
    mTransFormMatrix *= aTransform;
    mAABB = mAABB.transform(mTransFormMatrix);
}

void	Model::setAnimation(unsigned int animation, float timeInSeconds)
{
    if (mAnimated)
    {
        for (auto & mesh : mMeshes)
        {
            mesh->setAnimation(animation, timeInSeconds);
        }
    }
}