/*!
\file
\brief Header file of the ResourceManager class

This file contains declaration of the ResourceManager class and macros for the usage of this class as a singletone.
*/
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
#include <mutex>
#include <thread>
#include "Texture.hpp"

#define RESOURCES ResourceManager::getInstance()

class Shader;
class Texture;
class Model;
class Skybox;
/*!
\brief The class to manage resources

This class suppose to load, store and give access from different parts of code to shaders, textures, models, maps, fonts and othe resources of the project.
*/
class ResourceManager
{
public:
	/*!
	\brief Gives the instance of ResourceManager

	This function creates the instance of the ResourceManager class on the static memmory and return the reference to it.
	\return The reference to the instance of the ResourceManager class
	*/
	static ResourceManager   &getInstance();
	/*!
	\brief Sets the path to the executable

	This function sets the path to the executable of the program for generating relative path to the assets folder.
	\param[in] aPath The path to the executable
	*/
	void setBinFolder(std::string const& aPath);
	/*!
	\brief Gives the path to the executable

	This function gives the path to the executable of the program.
	\return The path to the executable
	*/
	std::string const &getBinFolder() const;
	/*!
	\brief Loads shader program to the resource manager

	This function takes pathes to vertex and fragmemt shaders, loads them and stores in the resource manager with the given name. 
	\param[in] vx The path to vertex shader
	\param[in] ft The path to fragment shader
	\param[in] name The name with which the shader program will be stored in resource manager
	*/
	void		loadShader(const GLchar* vx, const GLchar* ft, std::string const& name);
	/*!
	\brief Returns pointer to the shader program

	This function gives the access to the shader program with the asked name.
	\param[in] name The name of the shader program to give access to
	\return Shared ptr to the asked shader program
	*/
	std::shared_ptr<Shader>		getShader(std::string const& name);
	/*!
	\brief Loads texture to the resource manager

	This function takes path to texture, loads it and stores in the resource manager with the given name. 
	\param[in] path The path to texture
	\param[in] name The name with which the texture will be stored in resource manager
	\param[in] texType [optional] The type of the loaded texture
	*/
	void	loadTexture(const GLchar* path, std::string const& name, TextureType texType = TextureType::Diffuse);
	/*!
	\brief Returns pointer to the texture

	This function gives the access to the texture with the asked name.
	\param[in] name The name of the texture to give access to
	\return Shared ptr to the asked texture
	*/
	std::shared_ptr<Texture>	getTexture(std::string const& name);
	/*!
	\brief Loads model to the resource manager

	This function takes path to model, loads it and stores in the resource manager with the given name. 
	\param[in] path The path to model
	\param[in] name The name with which the model will be stored in resource manager
	\param[in] scale [optional] The basic scale of the model
	\param[in] offset [optional] The basic offset of the model
	\param[in] rotate [optional] The basic rotation axis of the model
	\param[in] angle [optional] The basic rotation angle of the model
	\param[in] glossiness [optional] The multiplier for model's specular component
	*/
	void                		loadModel(const GLchar* path, std::string const& name, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 offset = glm::vec3(.0f), glm::vec3 rotate = glm::vec3(.0f, 1.0f, .0f), float angle = .0f, float glossiness = .0f);
	/*!
	\brief Returns pointer to the model

	This function gives the access to the model with the asked name.
	\param[in] name The name of the model to give access to
	\return Shared ptr to the asked model
	*/
    std::shared_ptr<Model>		getModel(std::string const& name);
	/*!
	\brief Loads texture

	This function takes path to texture, loads it and and returns. 
	\param[in] path The path to texture
	\param[in] texType The type of the loaded texture
	\param[in] isModelTexture The flag which shows is it a model texture loading
	\return Shared ptr to the texture
	*/
    std::shared_ptr<Texture>	loadTextureFromFile(const GLchar* path, TextureType texType, bool isModelTexture = false);
	/*!
	\brief Loads texture from memory

	This function takes data of the texture, its size and type creates a texture and returns pointer to it. 
	\param[in] data The path to texture
	\param[in] texType The data of the texture
	\param[in] width The width of the loaded texture
	\param[in] height The height of the loaded texture
	\param[in] nrChannels The nubmer of channels of the loaded texture
	\param[in] isModelTexture The flag which shows is it a model texture loading
	\return Shared ptr to the texture
	*/
	std::shared_ptr<Texture> loadTextureFromMemory(unsigned char *data, TextureType texType, int width, int height, int nrChannels, bool isModelTexture);
	/*!
	\brief Loads skybox to the resource manager

	This function takes path to the skybox folder, loads it as a cubemap and stores in the resource manager. The name of the folder is also the alias for the skybox.
	\param[in] path The path to the skybox folder
	*/
    void		loadSkybox(std::string const& name);
	/*!
	\brief Returns pointer to the skybox

	This function gives the access to the skybox with the asked name.
	\param[in] name The name of the skybox to give access to
	\return Shared ptr to the asked skybox
	*/
    std::shared_ptr<Skybox>		getSkybox(std::string const& name);
	/*!
	\brief Clears resource manager

	This function deletes all resources that are owned by the resource manager
	*/
    void						clear();
	/*!
	\brief Loads map

	This function takes the filename of the map, loads it from the Assets/maps folder and returns.
	\param[in] aName The filename of the map.
	\return fstream of the loaded map
	*/
	std::ifstream getMap(std::string const& aName);
	/*!
	\brief Loads font

	This function takes the filename of the font, loads it from the Assets/font folder and returns.
	\param[in] path The filename of the map.
	\return Font, as a char vector
	*/
    std::vector<char> loadFont(std::string const& path);
	/*!
	\brief Process a little bit of textures
	\return whether did something useful
	*/
	bool tickLoading();
	/*!
	\brief Makes sure that async texture loading is done
	*/
	void endLoading();

	ResourceManager(ResourceManager const&) = delete;
	ResourceManager &operator=(ResourceManager const&) = delete;
private:
	ResourceManager();
	~ResourceManager();
	/*!
	\brief Loads shader program

	This function takes pathes to vertex and fragmemt shaders and loads them.
	\param[in] vx The path to vertex shader
	\param[in] ft The path to fragment shader
	\return Shared ptr to the loaded shader program
	*/
	std::shared_ptr<Shader>		loadShaderFromFile(const GLchar* vx, const GLchar* ft);
	/*!
	\brief Loads cubemap

	This function takes path to the cubemap, loads it and returns pointer to the cubemap, allocated on the graphics card.
	\param[in] path The path to the skybox folder
	\return Pointer to the cubemap, allocated on the graphics card.
	*/
    unsigned int loadCubemap(std::string const& aSkyboxName, std::shared_ptr<Skybox> tex);

private:
	/*!
	\brief The map of stored shaders
	*/
	std::map<std::string, std::shared_ptr<Shader>>		mShaders;
	/*!
	\brief The map of stored textures
	*/
	std::map<std::string, std::shared_ptr<Texture>>		mTextures;
	/*!
	\brief The map of stored models
	*/
	std::map<std::string, std::shared_ptr<Model>>		mModels;
	/*!
	\brief The map of stored skyboxes
	*/
	std::map<std::string, std::shared_ptr<Skybox>>		mSkyboxes;
	/*!
	\brief The path to the executable
	*/
	std::string mBinFolder;
	/*!
	\brief Cache for textures. you should query it by path to texture.
	*/
	std::map<std::string, std::shared_ptr<Texture>>		mTextureCache;


	struct SkyboxData {
		std::vector<uint8_t*>				data;
		std::vector<std::pair<int, int>>	sizes;
		std::shared_ptr<Skybox>	texture;
		GLenum					format;
	};

	struct TextureData {
		std::shared_ptr<Texture>	texture;
		GLint	width;
		GLint	height;
		uint8_t	*data;
		GLenum	format;
		bool	isModelTexture;
	};

	std::mutex					mTextureLock;
	std::vector<std::thread>	mTexReaders;
	std::vector<TextureData>	mPendingTextures;

	std::mutex					mSkyboxLock;
	std::vector<std::thread>	mSkyboxReaders;
	std::vector<SkyboxData>		mPendingSkyboxes;
};


#endif