//
// Created by Vadym KOZLOV on 7/7/18.
//
#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"

class Texture2D
{
public:
	Texture2D();
	~Texture2D();
	void generate(GLuint width, GLuint height, unsigned char* data);
	void bind() const;
	void setAlpha(bool);
private:
	GLuint	mID,
			mWidth, mHeight,
			mInternalFormat,
			mImageFormat,
			mWrapS,
			mWrapT,
			mFilterMin,
			mFilterMax;
	Texture2D(Texture2D const &);
	Texture2D &operator=(Texture2D const &);
};

#endif
