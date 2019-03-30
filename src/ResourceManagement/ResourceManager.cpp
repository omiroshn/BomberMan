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
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

std::shared_ptr<Skybox>		ResourceManager::loadSkybox(std::string const &aSkyboxName)
{
    //std::vector<unsigned char *> facesData;
    //std::vector<std::pair<int, int>> textureSizes;
    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    //for (unsigned int i = 0; i < cSkyboxFaces.size(); i++)
    //{
    //    std::string facePath(mBinFolder + "img/" + aSkyboxName + "/" + cSkyboxFaces[i]);
    //    unsigned char *data = stbi_load(facePath.c_str(), &width, &height, &nrChannels, 0);
    //    if (data)
    //    {
    //        facesData.push_back(data);
    //        textureSizes.push_back(std::make_pair(width, height));
    //        stbi_image_free(data);
    //    }
    //    else
    //    {
    //        throw CustomException("Skybox texture failed to load at path[" + std::string(facePath) + "]");
    //        stbi_image_free(data);
    //    }
    //}

    std::shared_ptr<Skybox> skybox;
    try
    {
        skybox = std::make_shared<Skybox>();
        //skybox->generate(facesData, textureSizes);
        skybox->mCubeMap = loadCubemap(aSkyboxName);
        mSkyboxes.emplace(aSkyboxName, skybox);
    }
    catch (...)
    {
        throw CustomException("Something happened when loading skybox [" + std::string(aSkyboxName) + "]");
    }
    return skybox;
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
#if defined (COPY_ASSETS)
    mBinFolder = aPath + "./Assets/";
#elif defined(CLION_ASSETS)
    mBinFolder = "../../Assets/";
#else
    mBinFolder = "../Assets/";
#endif
}

ResourceManager::ResourceManager()
{

};

ResourceManager::~ResourceManager()
{
	clear();
};
