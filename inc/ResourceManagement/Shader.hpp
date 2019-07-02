#ifndef Shader_H
# define Shader_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "GL/glew.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <map>

class Shader
{
public:
	Shader(std::string const &vertexSrcPath, std::string const &fragmentSrcPath);
	~Shader();
	Shader();

public:
	void use();
	void setBool(const std::string &, bool ) const;
	void setInt(const std::string &, int ) const;
	void setFloat(const std::string &, float ) const;
	void setVec2(const std::string &, const glm::vec2 &) const;
	void setVec2(const std::string &, float, float) const;
	void setVec3(const std::string &, const glm::vec3 &) const;
	void setVec3(const std::string &, float, float, float) const;
	void setVec4(const std::string &, const glm::vec4 &) const;
	void setVec4(const std::string &, float, float, float, float) const;
	void setMat2(const std::string &, const glm::mat2 &) const;
	void setMat3(const std::string &, const glm::mat3 &) const;
	void setMat4(const std::string &, const glm::mat4 &) const;
	bool isShaderCompiled(unsigned int, int, char *) const;
	bool isProgramLinked(unsigned int, int, char *) const;
	unsigned int mShaderProgram;

private:
    GLuint getUniformLocation(std::string const&) const;
    mutable std::map<std::string, GLuint> mUniformLocations;
};

#endif