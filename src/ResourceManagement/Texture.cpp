//
// Created by Vadym KOZLOV on 7/7/18.
//
#include "ResourceManagement/Texture.hpp"
#include "CustomException.hpp"

std::map <TextureType, std::string> cTextureTypesString
        {
                {TextureType::Diffuse, "texture_diffuse"},
                {TextureType::Specular, "texture_specular"},
                {TextureType::Normal, "texture_normal"},
                {TextureType::Height, "texture_height"}
        };

std::map <std::string, TextureType> cStringTextureTypes
        {
                { "texture_diffuse", TextureType::Diffuse},
                { "texture_specular", TextureType::Specular},
                { "texture_normal", TextureType::Normal},
                { "texture_height", TextureType::Height}
        };

Texture::Texture(TextureType aType)
		: mType(aType), mWidth(0), mHeight(0), mInternalFormat(GL_RGBA), mImageFormat(GL_RGB), mWrapS(GL_REPEAT), mWrapT(GL_REPEAT), mFilterMin(GL_LINEAR), mFilterMax(GL_LINEAR)
{
	glGenTextures(1, &mID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
};

Texture::Texture(Texture const &)
{

};

Texture &Texture::operator=(Texture const &)
{
	return *this;
};

void Texture::generate(GLuint width, GLuint height, unsigned char* data, GLenum format, GLuint filterMin, GLuint filerMax)
{
	mWidth = width;
	mHeight = height;
    mImageFormat = format;
    mInternalFormat = format;
    mFilterMin = filterMin;
    mFilterMax = filerMax;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mImageFormat, GL_UNSIGNED_BYTE, data);
    if (mFilterMin == GL_LINEAR_MIPMAP_LINEAR)
        glGenerateMipmap(GL_TEXTURE_2D);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterMax);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, mID);
}

std::string Texture::getTextureType() const
{
	return cTextureTypesString[mType];
}

GLuint Texture::getTextureID()
{
	return mID;
}

 std::string Texture::getTextureTypeStringFromType(TextureType type)
{
	return cTextureTypesString[type];
}

TextureType Texture::getTextureTypeFromString(std::string typeString)
{
	if (cStringTextureTypes.count(typeString))
		return cStringTextureTypes[typeString];
	else
		throw CustomException("No such texture type");
}
