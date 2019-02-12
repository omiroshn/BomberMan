//
// Created by Vadym KOZLOV on 7/7/18.
//

#ifndef NewResourceManager_HPP
# define NewResourceManager_HPP

#include <map>
#include <string>

#include "GL/glew.h"
#include "renderer/Texture.hpp"
#include "renderer/Shader.hpp"
class NewResourceManager
{
public:
	static NewResourceManager   &getInstance();
	void setBinFolder(std::string const& aPath);
	std::shared_ptr<Shader>		loadShader(const GLchar *, const GLchar *, std::string const &);
	std::shared_ptr<Shader>		getShader(std::string const &);
	std::shared_ptr<Texture2D>	loadTexture(const GLchar *, std::string const &, bool);
	std::shared_ptr<Texture2D>	getTexture(std::string const &name);
	void						clear();
private:
	NewResourceManager();
	~NewResourceManager();
	NewResourceManager(NewResourceManager const &) = delete;
	NewResourceManager &operator=(NewResourceManager const &) = delete;
	std::shared_ptr<Shader>		loadShaderFromFile(const GLchar *, const GLchar *);
	std::shared_ptr<Texture2D>	loadTextureFromFile(const GLchar*, bool);
private:
	std::map<std::string, std::shared_ptr<Shader>>			mShaders;
	std::map<std::string, std::shared_ptr<Texture2D>>		mTextures;
	std::string mBinFolder;
};


#endif