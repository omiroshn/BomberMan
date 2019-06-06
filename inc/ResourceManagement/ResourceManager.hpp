//
// Created by Vadym KOZLOV on 7/7/18.
//

#ifndef ResourceManager_HPP
# define ResourceManager_HPP

#include <map>
#include <string>
#include <memory>
#include "GL/glew.h"

#define RESOURCES ResourceManager::getInstance()

class Shader;
class Texture;
class Model;
class Skybox;
class ZipHolder;
class ResourceManager
{
public:
	static ResourceManager   &getInstance();
	void setBinFolder(std::string const& aPath);
    std::string const& getBinFolder() const;
	std::shared_ptr<Shader>		loadShader(const GLchar *, const GLchar *, std::string const &);
	std::shared_ptr<Shader>		getShader(std::string const &);
	std::shared_ptr<Texture>	loadTexture(const GLchar *, std::string const &, std::string const &texType = "texture_diffuse");
	std::shared_ptr<Texture>	getTexture(std::string const &name);
	std::shared_ptr<Model>		loadModel(const GLchar *, std::string const &);
    std::shared_ptr<Model>		getModel(std::string const &name);
    std::shared_ptr<Texture>	loadTextureFromFile(const GLchar*, std::string const &, bool isModelTexture = false);
    std::shared_ptr<Skybox>		loadSkybox(std::string const &);
    std::shared_ptr<Skybox>		getSkybox(std::string const &);
    void						clear();
    void						loadZip(std::string const &path);
    void						loadFileFromZip(std::string const & filePathInZip);
	ResourceManager(ResourceManager const &) = delete;
	ResourceManager &operator=(ResourceManager const &) = delete;
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

	std::string mBinFolder;

	std::unique_ptr<ZipHolder> mZH{nullptr};
};


#endif