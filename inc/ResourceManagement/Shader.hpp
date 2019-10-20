#ifndef Shader_H
# define Shader_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "GL/glew.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <map>
/*!
\brief The representation of Shader.

Holds shader program and provides useful methods for setting Uniforms.
*/
class Shader
{
public:
	Shader(std::string const &vertexSrcPath, std::string const &fragmentSrcPath);
	~Shader();
	Shader();
	Shader& operator=(const Shader&) = default;

public:
/*!
\brief Binds this shader for further use.
*/
	void use();
/*!
\brief Sets uniform of type Int on this Shader.
*/
	void setInt(const std::string &, int ) const;
/*!
\brief Sets uniform of type Float on this Shader.
*/
	void setFloat(const std::string &, float ) const;
/*!
\brief Sets uniform of type vec3 on this Shader.
*/
	void setVec3(const std::string &, const glm::vec3 &) const;
/*!
\brief Sets uniform of type mat4 on this Shader.
*/
	void setMat4(const std::string &, const glm::mat4 &) const;

private:
    GLuint getUniformLocation(std::string const&) const;
    mutable std::map<std::string, GLuint> mUniformLocations;
	static unsigned int sBoundProgram;

	bool isShaderCompiled(unsigned int, int, char *) const;
	bool isProgramLinked(unsigned int, int, char *) const;
	unsigned int mShaderProgram;
};

#endif