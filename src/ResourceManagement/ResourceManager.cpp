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

unsigned int ResourceManager::loadCubemap(std::string const& aSkyboxName, std::shared_ptr<Skybox> tex)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	mSkyboxReaders.emplace_back([this, localName = std::string(aSkyboxName), tex]() {
		SkyboxData skybox;
		skybox.texture = tex;
		skybox.format = GL_RGB;
		for (unsigned int i = 0; i < cSkyboxFaces.size(); i++)
		{
			int width, height, nrChannels;
			std::string facePath(mBinFolder + "img/" + localName + "/" + cSkyboxFaces[i]);
			unsigned char* data = stbi_load(facePath.c_str(), &width, &height, &nrChannels, 3);
			if (!data)
			{
				std::cout << "Cubemap texture failed to load at path: " << facePath << std::endl;
				return;
			}
			skybox.data.push_back(data);
			skybox.sizes.emplace_back(width, height);
		}
		mSkyboxLock.lock();
		mPendingSkyboxes.push_back(skybox);
		mSkyboxLock.unlock();
	});

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return textureID;
}

void ResourceManager::endLoading()
{
	std::vector<TextureData> localTex;
	std::vector<SkyboxData> localSky;
	mTextureLock.lock();
	std::swap(localTex, mPendingTextures);
	mTextureLock.unlock();

	for (auto& It : localTex)
		It.texture->generate(It.width, It.height, It.data, It.format, It.isModelTexture ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	mSkyboxLock.lock();
	std::swap(localSky, mPendingSkyboxes);
	mSkyboxLock.unlock();

	for (auto& It : localSky)
		It.texture->generate(It.data, It.sizes, It.format);

	for (auto& It : mTexReaders)
		It.join();
	for (auto& It : mSkyboxReaders)
		It.join();
	
	for (auto& It : mPendingTextures)
		It.texture->generate(It.width, It.height, It.data, It.format, It.isModelTexture ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	for (auto& It : mPendingSkyboxes)
		It.texture->generate(It.data, It.sizes, It.format);

	mTexReaders.clear();
	mSkyboxReaders.clear();
	mPendingTextures.clear();
	mPendingSkyboxes.clear();
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

void ResourceManager::loadTexture(const GLchar *file, std::string const &name, TextureType texType)
{
	mTextures.emplace(name, loadTextureFromFile(std::string(mBinFolder + "img/" + std::string(file)).c_str(), texType));
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

std::shared_ptr<Texture> ResourceManager::loadTextureFromMemory(unsigned char *data, TextureType texType, int width, int height, int nrChannels, bool isModelTexture)
{
    std::shared_ptr<Texture> texture;
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else
        format = GL_RGBA;

    texture = std::make_shared<Texture>(texType);
    if (isModelTexture)
        texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data, format, GL_LINEAR_MIPMAP_LINEAR);
    else
        texture->generate(static_cast<GLuint>(width), static_cast<GLuint>(height), data, format);
    return texture;
}

std::shared_ptr<Texture> ResourceManager::loadTextureFromFile(const GLchar *file, TextureType texType, bool isModelTexture)
{
	auto It = mTextureCache.find(file);
	if (It != mTextureCache.end())
	{
		return It->second;
	}
	auto texture = std::make_shared<Texture>(texType);
	mTexReaders.emplace_back([this, texture, texType, isModelTexture](std::string file) {
		TextureData texData;
		texData.isModelTexture = isModelTexture;
		texData.texture = texture;

		int nrChannels;
		texData.data = stbi_load(file.c_str(), &texData.width, &texData.height, &nrChannels, 0);

		if (nrChannels == 1)
			texData.format = GL_RED;
		else if (nrChannels == 3)
			texData.format = GL_RGB;
		else
			texData.format = GL_RGBA;

		mTextureLock.lock();
		mPendingTextures.emplace_back(texData);
		mTextureLock.unlock();
	}, std::string(file));
    return texture;
};

void    ResourceManager::loadSkybox(std::string const &aSkyboxName)
{
    std::shared_ptr<Skybox> skybox;
    try
    {
        skybox = std::make_shared<Skybox>();
        skybox->mCubeMap = loadCubemap(aSkyboxName, skybox);

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

std::ifstream ResourceManager::getMap(std::string const & aName)
{
    std::ifstream f(mBinFolder + "maps/" + aName);
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

bool ResourceManager::tickLoading()
{
	if (!mPendingTextures.empty())
	{
		mTextureLock.lock();
		auto It = mPendingTextures[mPendingTextures.size() - 1];
		mPendingTextures.pop_back();
		mTextureLock.unlock();
		It.texture->generate(It.width, It.height, It.data, It.format, It.isModelTexture ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		return true;
	}

	if (!mPendingSkyboxes.empty())
	{
		mSkyboxLock.lock();
		auto It = mPendingSkyboxes[mPendingSkyboxes.size() - 1];
		mPendingSkyboxes.pop_back();
		mSkyboxLock.unlock();
		It.texture->generate(It.data, It.sizes, It.format);
		return true;
	}
	return false;
}
