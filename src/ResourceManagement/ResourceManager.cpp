#include "ResourceManagement/ResourceManager.hpp"
#include "Core.hpp"
#include "CustomException.hpp"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
# ifndef STBI_INCLUDE_STB_IMAGE_H
#  define STB_IMAGE_IMPLEMENTATION
#  include "stb_image.h"
# endif
#endif

std::vector<std::string> const cSkyboxFaces
{
    "right.png",
    "left.png",
    "top.png",
    "bottom.png",
    "front.png",
    "back.png"
};

unsigned int ResourceManager::loadCubemap(std::string const & aSkyboxName)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < cSkyboxFaces.size(); i++)
    {
        std::string facePath(mBinFolder + "img/" + aSkyboxName + "/" + cSkyboxFaces[i]);
        unsigned char *data = stbi_load(facePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << facePath << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

ResourceManager &ResourceManager::getInstance()
{
	static ResourceManager manager;
	return manager;
}

void ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string const &name)
{
    mShaders.emplace(name, loadShaderFromFile(std::string(mBinFolder + "shader/" + vShaderFile).c_str(), std::string(mBinFolder + "shader/" + fShaderFile).c_str()));
}

std::shared_ptr<Shader> ResourceManager::getShader(std::string const &name)
{
	auto It = mShaders.find(name);
	if (It != mShaders.end())
		return It->second;
	else
		throw CustomException("No such shader: \""  + name + "\"");
}

void ResourceManager::loadTexture(const GLchar *file, std::string const &name, std::string const &texType)
{
	mTextures.emplace(name, loadTextureFromFile(std::string(mBinFolder + "img/" + file).c_str(), texType));
};

std::shared_ptr<Texture> ResourceManager::getTexture(std::string const &name)
{
	auto It = mTextures.find(name);
	if (It != mTextures.end())
		return It->second;
	else
		throw CustomException("No such texture: \""  + name + "\"");
};


void ResourceManager::loadModel(const GLchar *file, std::string const &name, glm::vec3 scale, glm::vec3 offset, glm::vec3 axis, float angle, float glossiness)
{
	mModels.emplace(name, std::make_shared<Model>(mBinFolder + "models/" + file, scale, offset, axis, angle, glossiness));
};

std::shared_ptr<Model> ResourceManager::getModel(std::string const &name)
{
	auto It = mModels.find(name);
	if (It != mModels.end())
		return It->second;
	else
		throw CustomException("No such model: \"" + name + "\"");
};

void ResourceManager::clear()
{
	mTextures.clear();
	mShaders.clear();
	mModels.clear();
    mSkyboxes.clear();
};

std::shared_ptr<Shader> ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile)
{
	std::shared_ptr<Shader> shader;
	try
	{
		shader = std::make_shared<Shader>(vShaderFile, fShaderFile);
	}
	catch (std::exception& e)
	{
        std::cerr << "Error:" << e.what() <<  " when loading shader [" << std::string(vShaderFile) << ";" << std::string(fShaderFile) << "]";
    }
	return shader;
};

std::shared_ptr<Texture> ResourceManager::loadTextureFromMemory(unsigned char *data, std::string const &texType, int width, int height, int nrChannels, bool isModelTexture)
{
    std::shared_ptr<Texture> texture;
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else
        format = GL_RGBA;

    texture = std::make_shared<Texture>(Texture::getTextureTypeFromString(texType));
    if (isModelTexture)
        texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data, format, GL_LINEAR_MIPMAP_LINEAR);
    else
        texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data, format);
    return texture;
}

std::shared_ptr<Texture> ResourceManager::loadTextureFromFile(const GLchar *file, std::string const &texType, bool isModelTexture)
{
	auto It = mTextureCache.find(file);
	if (It != mTextureCache.end())
	{
		return It->second;
	}

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (!data)
        throw CustomException("Something happened when loading texture[" + std::string(file) + "]");
    auto result = loadTextureFromMemory(data, texType, width, height, nrChannels, isModelTexture);
    stbi_image_free(data);
    return result;
};

void    ResourceManager::loadSkybox(std::string const &aSkyboxName)
{
    std::shared_ptr<Skybox> skybox;
    try
    {
        skybox = std::make_shared<Skybox>();
        skybox->mCubeMap = loadCubemap(aSkyboxName);
        mSkyboxes.emplace(aSkyboxName, skybox);
    }
    catch (...)
    {
        throw CustomException("Something happened when loading skybox [" + std::string(aSkyboxName) + "]");
    }
}

std::shared_ptr<Skybox>		ResourceManager::getSkybox(std::string const &name)
{
    if (mSkyboxes.count(name))
        return mSkyboxes[name];
    else
        throw CustomException("No such skybox: \"" + name + "\"");
}

void ResourceManager::setBinFolder(std::string const &aPath)
{
    mBinFolder = aPath + "/Assets/";
}

std::string const& ResourceManager::getBinFolder() const
{
    return mBinFolder;
}

ResourceManager::ResourceManager()
{

};

ResourceManager::~ResourceManager()
{
	clear();
};

std::fstream ResourceManager::getMap(std::string const & aName)
{
    std::fstream f(mBinFolder + "maps/" + aName);
    return f;
}

std::vector<char> ResourceManager::loadFont(std::string const &path)
{
    std::ifstream infile(mBinFolder + "font/" + path, std::ios::binary);
    infile.seekg(0, std::ios::end);
    size_t file_size_in_byte = infile.tellg();
    std::vector<char> data;
    data.resize(file_size_in_byte);
    infile.seekg(0, std::ios::beg);
    infile.read(&data[0], file_size_in_byte);
    return data;
}