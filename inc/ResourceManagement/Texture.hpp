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
	Height,
	Font,
	Depth
};


class Texture
{
public:
	Texture(TextureType aType, GLuint width, GLuint height, unsigned char *data, GLenum format, GLuint filterMin, GLuint filerMax);
	Texture(TextureType aType = TextureType::Diffuse);
	Texture(Texture const &) = delete;
	Texture &operator=(Texture const &) = delete;
	~Texture();
	void generate(GLuint width, GLuint height, unsigned char* data, GLenum format, GLuint filterMin = GL_LINEAR, GLuint filerMax = GL_LINEAR);
	void bind() const;
	std::string getTextureType() const;

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
};

#endif
