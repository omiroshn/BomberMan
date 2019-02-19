//
// Created by Vadym KOZLOV on 7/7/18.
//
#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include <map>
#include <string>
#include <vector>

enum class TextureType
{
	Diffuse = 0,
	Specular,
	Normal,
	Height
};


class Texture
{
public:
	Texture(TextureType aType = TextureType::Diffuse);
	~Texture();
	void generate(GLuint width, GLuint height, unsigned char* data);
	void bind() const;
	void setAlpha(bool);
	std::string getTextureType() const;
    static std::string getTextureTypeStringFromType(TextureType);
    static TextureType getTextureTypeFromString(std::string);

    GLuint getTextureID();
private:
	GLuint	mID,
			mWidth, mHeight,
			mInternalFormat,
			mImageFormat,
			mWrapS,
			mWrapT,
			mFilterMin,
			mFilterMax;
	TextureType mType;
	Texture(Texture const &);
	Texture &operator=(Texture const &);
};

#endif
