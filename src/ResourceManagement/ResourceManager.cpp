#include "ResourceManagement/ResourceManager.hpp"
#include "Core.hpp"
#include "CustomException.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
# ifndef STBI_INCLUDE_STB_IMAGE_H
#  define STB_IMAGE_IMPLEMENTATION
#  include "stb_image.h"
# endif
#endif

ResourceManager &ResourceManager::getInstance()
{
	static ResourceManager manager;
	return manager;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string const &name)
{
	mShaders.emplace(name, loadShaderFromFile(std::string(mBinFolder + "shader/" + vShaderFile).c_str(), std::string(mBinFolder + "shader/" + fShaderFile).c_str()));
	return mShaders[name];
}

std::shared_ptr<Shader> ResourceManager::getShader(std::string const &name)
{
	if (mShaders.count(name))
		return mShaders[name];
	else
		throw CustomException("No such shader: \""  + name + "\"");
}

std::shared_ptr<Texture> ResourceManager::loadTexture(const GLchar *file, std::string const &name, std::string const &texType)
{
	mTextures.emplace(name, loadTextureFromFile(std::string(mBinFolder + "img/" + file).c_str(), texType));
	return mTextures[name];
};

std::shared_ptr<Texture> ResourceManager::getTexture(std::string const &name)
{
	if (mTextures.count(name))
		return mTextures[name];
	else
		throw CustomException("No such texture: \""  + name + "\"");
};


std::shared_ptr<Model> ResourceManager::loadModel(const GLchar *file, std::string const &name)
{
	mModels.emplace(name, std::make_shared<Model>(mBinFolder + "models/" + file));
	return mModels[name];
};

std::shared_ptr<Model> ResourceManager::getModel(std::string const &name)
{
	if (mModels.count(name))
		return mModels[name];
	else
		throw CustomException("No such model: \"" + name + "\"");
};

void ResourceManager::clear()
{
	mTextures.clear();
	mShaders.clear();
	mModels.clear();
};

std::shared_ptr<Shader> ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile)
{
	std::shared_ptr<Shader> shader;
	try
	{
		shader = std::make_shared<Shader>(vShaderFile, fShaderFile);
	}
	catch (...)
	{
        throw CustomException("Something happened when loading shader [" + std::string(vShaderFile) + ";" + std::string(fShaderFile) + "]");
    }
	return shader;
};

std::shared_ptr<Texture> ResourceManager::loadTextureFromFile(const GLchar *file, std::string const &texType, bool isModelTexture)
{
	std::shared_ptr<Texture> texture;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

		texture = std::make_shared<Texture>(Texture::getTextureTypeFromString(texType));
        if (isModelTexture)
            texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data, format, GL_LINEAR_MIPMAP_LINEAR);
        else
            texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data, format);
		stbi_image_free(data);
	}
	else
	{
        throw CustomException("Something happened when loading texture[" + std::string(file) + "]");
	}
	return texture;
};

void ResourceManager::setBinFolder(std::string const &aPath)
{
    mBinFolder = aPath + "./Assets/";
}

ResourceManager::ResourceManager()
{

};

ResourceManager::~ResourceManager()
{
	clear();
};
