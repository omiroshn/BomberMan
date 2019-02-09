//
// Created by Vadym KOZLOV on 8/11/18.
//

#include "Character.hpp"

Character::Character(FT_Face face)
{
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	mSize = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
	mBearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	mAdvance = static_cast<GLuint>(face->glyph->advance.x);
};

Character::~Character()
{
	glDeleteTextures(1, &mTextureID);

};

Character::Character()
{

};

Character::Character(Character const &)
{
};

Character &Character::operator=(Character const &)
{
	return *this;
};