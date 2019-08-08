#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "CustomException.hpp"
#include "Utilities/AnimationUtils.h"
#include "ResourceManagement/Animation.h"

Model::Model(std::string const &path, glm::vec3 scale, glm::vec3 offset, glm::vec3 axis, float angle, float glossiness)
	: mTransFormMatrix(glm::mat4(1.0f)), mImporter(new Assimp::Importer()), mGlossiness(glossiness)
{
    loadModel(path);
    makeUnitModel();
    glm::mat4 basicTransform = glm::mat4(1.0f);
    basicTransform = glm::scale(basicTransform, scale);
    basicTransform = glm::translate(basicTransform, offset);
    basicTransform = glm::rotate(basicTransform, glm::radians(angle), axis);
    transform(basicTransform);
}

Model::~Model()
{
    delete mImporter;
}

void Model::makeUnitModel()
{
    auto size = mAABB.size();
    auto scale_factor = 1.0f / std::max(std::max(size.x, size.y), size.z);
    glm::mat4 basicTransform = glm::mat4(1.0f);
    basicTransform = glm::scale(basicTransform, glm::vec3(scale_factor));
	basicTransform = glm::translate(basicTransform, -mAABB.getCenter());
    transform(basicTransform);
}

void Model::loadModel(std::string const &path)
{
    auto const* scene = mImporter->ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
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
    std::vector<glm::mat4> offsets;
    offsets.resize(mesh->mNumBones);
    std::map<std::string, unsigned int> bones{processBones(mesh, offsets, vertices)};
    mMeshes.emplace_back(std::make_unique<Mesh>(vertices, indices, textures, bones, offsets, scene, mGlossiness));
}

std::map<std::string, unsigned int> Model::processBones(aiMesh const* mesh, std::vector<glm::mat4> & aOffsets, std::vector<Vertex> & vertices)
{
    std::map<std::string, unsigned int> bones;
    unsigned int        totalBones{0};
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
		auto *bone = mesh->mBones[i];
        unsigned int boneIndex;
        std::string boneName(mesh->mBones[i]->mName.data);

        if (bones.find(boneName) == bones.end())
            bones[boneName] = totalBones++;
        boneIndex = bones[boneName];
        aOffsets[boneIndex] = AnimationUtils::aiMatToGlmMat(mesh->mBones[i]->mOffsetMatrix);
		for (unsigned int j = 0; j < bone->mNumWeights; j++)
        {
			aiVertexWeight weight = bone->mWeights[j];
			for (unsigned int k = 0; k < 3; k++)
            {
				if (vertices[weight.mVertexId].Weighs[k] == 0)
                {
					vertices[weight.mVertexId].Weighs[k] = weight.mWeight;
					vertices[weight.mVertexId].BonesID[k] = i;
					break;
				}
			}
		}
	}
    return bones;
}

std::vector<Vertex> Model::loadVertices(aiMesh const* mesh)
{
    std::vector<Vertex> vertices{};

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex = Vertex();
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        if (mesh->HasNormals())
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
			vertex.Normal = glm::normalize(vertex.Normal);
        }
        if (mesh->HasTangentsAndBitangents())
        {
            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;
            vertex.Bitangent.x = mesh->mBitangents[i].x;
            vertex.Bitangent.y = mesh->mBitangents[i].y;
            vertex.Bitangent.z = mesh->mBitangents[i].z;
        }
		else
		{
			glm::vec3 tmp = glm::cross(vertex.Normal, glm::vec3(0,1,0));
			if (glm::length(tmp) < 0.001)
				tmp = glm::cross(glm::vec3(1, 0, 0), vertex.Normal);
			vertex.Tangent = tmp;
			vertex.Bitangent = glm::cross(tmp, vertex.Normal);
		}
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
    return vertices;
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
    return indices;
}

std::vector<std::shared_ptr<Texture>> Model::loadTextures(aiMesh const* mesh, aiScene const* scene)
{
    std::vector<std::shared_ptr<Texture>> textures;

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    return textures;
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texFullPath(mDirectory + "/" + std::string(str.C_Str()));
        size_t pos = texFullPath.find("\\");
        while (pos != std::string::npos)
        {
            texFullPath.replace(pos, 1, "/");
            pos = texFullPath.find("\\", pos + 1);
        }
        auto texture = RESOURCES.loadTextureFromFile(texFullPath.c_str(), typeName, true);
        textures.push_back(texture);
    }
    return textures;
}


void Model::draw(std::shared_ptr<Shader> const& shader, std::vector<glm::mat4> const& transforms)
{
    for(auto &mesh : mMeshes)
    {
        if (mAnimated)
            mesh->doAnimation();
        mesh->draw(shader, transforms, mTransFormMatrix);
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

void	Model::setAnimation(Animation const& anim)
{
    if (mAnimated)
    {
        for (auto & mesh : mMeshes)
        {
            mesh->setAnimation(anim);
        }
    }
}