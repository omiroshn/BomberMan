#include "NewResourceManager.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
# ifndef STBI_INCLUDE_STB_IMAGE_H
#  define STB_IMAGE_IMPLEMENTATION
#  include "stb_image.h"
# endif
#endif

NewResourceManager &NewResourceManager::getInstance()
{
	static NewResourceManager manager;
	return manager;
}

std::shared_ptr<Shader> NewResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string const &name)
{
	mShaders.emplace(name, loadShaderFromFile(std::string(mBinFolder + "shader/" + vShaderFile).c_str(), std::string(mBinFolder + "shader/" + fShaderFile).c_str()));
	return mShaders[name];
}

std::shared_ptr<Shader> NewResourceManager::getShader(std::string const &name)
{
	return mShaders[name];
}

std::shared_ptr<Texture2D> NewResourceManager::loadTexture(const GLchar *file, std::string const &name, bool isAlpha)
{
	mTextures.emplace(name, loadTextureFromFile(std::string(mBinFolder + + "img/" + file).c_str(), isAlpha));
	return mTextures[name];
};

std::shared_ptr<Texture2D> NewResourceManager::getTexture(std::string const &name)
{
	return mTextures[name];
};

void NewResourceManager::clear()
{
	mTextures.clear();
	mShaders.clear();
};

std::shared_ptr<Shader> NewResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile)
{
	std::shared_ptr<Shader> shader;
	try
	{
		shader = std::make_shared<Shader>(vShaderFile, fShaderFile);
	}
	catch (CustomException &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Exception: Something happened when loading shader [" <<vShaderFile << ";" << fShaderFile << "]" << std::endl;
	}
	return shader;
};

std::shared_ptr<Texture2D> NewResourceManager::loadTextureFromFile(const GLchar *file, bool isAlpha)
{
	std::shared_ptr<Texture2D> texture;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		texture = std::make_shared<Texture2D>();
		texture->setAlpha(isAlpha);
		texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data);
		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Exception: Something happened when loading texture [" << file << "]" << std::endl;
	}
	return texture;
};

void NewResourceManager::setBinFolder(std::string const &aPath)
{
    mBinFolder = aPath + "../Assets/";
}

NewResourceManager::NewResourceManager()
{

};

NewResourceManager::~NewResourceManager()
{
	clear();
};
