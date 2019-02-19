//
// Created by Vadym KOZLOV on 7/7/18.
//

#ifndef ResourceManager_HPP
# define ResourceManager_HPP

#include <map>
#include <string>
#include <memory>
#include "GL/glew.h"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"

#define RESOURCES ResourceManager::getInstance()

class ResourceManager
{
public:
	static ResourceManager   &getInstance();
	void setBinFolder(std::string const& aPath);
	std::shared_ptr<Shader>		loadShader(const GLchar *, const GLchar *, std::string const &);
	std::shared_ptr<Shader>		getShader(std::string const &);
	std::shared_ptr<Texture>	loadTexture(const GLchar *, std::string const &, bool, std::string const &texType = "texture_diffuse");
	std::shared_ptr<Texture>	getTexture(std::string const &name);
	std::shared_ptr<Model>		loadModel(const GLchar *, std::string const &);
	std::shared_ptr<Model>		getModel(std::string const &name);

    std::shared_ptr<Texture>	loadTextureFromFile(const GLchar*, bool, std::string const &);
    void						clear();
	ResourceManager(ResourceManager const &) = delete;
	ResourceManager &operator=(ResourceManager const &) = delete;
private:
	ResourceManager();
	~ResourceManager();
	std::shared_ptr<Shader>		loadShaderFromFile(const GLchar *, const GLchar *);
private:
	std::map<std::string, std::shared_ptr<Shader>>		mShaders;
	std::map<std::string, std::shared_ptr<Texture>>		mTextures;
	std::map<std::string, std::shared_ptr<Model>>		mModels;

	std::string mBinFolder;
};


#endif