//
// Created by Vadym KOZLOV on 7/7/18.
//

#ifndef ResourceManager_HPP
# define ResourceManager_HPP

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

#define RESOURCES ResourceManager::getInstance()

class Shader;
class Texture;
class Model;
class Skybox;
class ResourceManager
{
public:
	static ResourceManager   &getInstance();
	void setBinFolder(std::string const& aPath);
	std::string const &getBinFolder() const;
	void		loadShader(const GLchar *, const GLchar *, std::string const &);
	std::shared_ptr<Shader>		getShader(std::string const &);
	void	loadTexture(const GLchar *, std::string const &, std::string const &texType = "texture_diffuse");
	std::shared_ptr<Texture>	getTexture(std::string const &name);
	void                		loadModel(const GLchar *, std::string const &, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 offset = glm::vec3(.0f), glm::vec3 rotate = glm::vec3(.0f, 1.0f, .0f), float angle = .0f);
    std::shared_ptr<Model>		getModel(std::string const &name);
    std::shared_ptr<Texture>	loadTextureFromFile(const GLchar*, std::string const &, bool isModelTexture = false);
	std::shared_ptr<Texture>	loadTextureFromMemory(unsigned char *data, std::string const &texType, int width, int height, int nrChannels, bool isModelTexture);
	void		loadSkybox(std::string const &);
    std::shared_ptr<Skybox>		getSkybox(std::string const &);
    void						clear();
	ResourceManager(ResourceManager const &) = delete;
	ResourceManager &operator=(ResourceManager const &) = delete;

	std::fstream getMap(std::string const & aName);
    std::vector<char> loadFont(std::string const &path);

private:
	ResourceManager();
	~ResourceManager();
	std::shared_ptr<Shader>		loadShaderFromFile(const GLchar *, const GLchar *);
    unsigned int loadCubemap(std::string const & aSkyboxName);

private:
	std::map<std::string, std::shared_ptr<Shader>>		mShaders;
	std::map<std::string, std::shared_ptr<Texture>>		mTextures;
	std::map<std::string, std::shared_ptr<Model>>		mModels;
    std::map<std::string, std::shared_ptr<Skybox>>		mSkyboxes;
	//std::string                                         
	std::string mBinFolder;
};


#endif