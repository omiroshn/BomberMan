//
// Created by Vadym KOZLOV on 7/7/18.
//

#ifndef ResourceManager_HPP
#define ResourceManager_HPP

#include <map>
#include <string>

#include "GL/glew.h"
#include "Texture.hpp"
#include "Shader.hpp"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	std::shared_ptr<Shader>		loadShader(const GLchar *, const GLchar *, std::string const &);
	std::shared_ptr<Shader>		getShader(std::string const &);
	std::shared_ptr<Texture2D>	loadTexture(const GLchar *, std::string const &, bool);
	std::shared_ptr<Texture2D>	getTexture(std::string const &name);
	void						clear();
private:
	ResourceManager(ResourceManager const &);
	ResourceManager &operator=(ResourceManager const &);
	std::shared_ptr<Shader>		loadShaderFromFile(const GLchar *, const GLchar *);
	std::shared_ptr<Texture2D>	loadTextureFromFile(const GLchar*, bool);
private:
	std::map<std::string, std::shared_ptr<Shader>>			mShaders;
	std::map<std::string, std::shared_ptr<Texture2D>>		mTextures;
};

#endif