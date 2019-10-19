/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThirdAPI.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 22:30:00 by vkozlov           #+#    #+#             */
/*   Updated: 2018/06/25 22:30:00 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/Shader.hpp"
#include "CustomException.hpp"

unsigned int Shader::sBoundProgram;

Shader::Shader(std::string const &vertexSrcPath, std::string const &fragmentSrcPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexSrcPath);
		fShaderFile.open(fragmentSrcPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode   = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure &)
	{
		throw CustomException("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}
	const char* vertexShaderSrc = vertexCode.c_str();
	const char* fragmentShaderSrc = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);

	char errLog[512];
	if (!isShaderCompiled(vertexShader, 512, errLog))
		throw CustomException(std::string("VERTEX_SHADER::COMPILATION ") + errLog);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);
	if (!isShaderCompiled(fragmentShader, 512, errLog))
		throw CustomException(std::string("FRAGMENT_SHADER::COMPILATION ") + errLog);
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, vertexShader);
	glAttachShader(mShaderProgram, fragmentShader);
	glLinkProgram(mShaderProgram);
	if (!isProgramLinked(mShaderProgram, 512, errLog))
		throw CustomException(std::string("PROGRAM::LINKING ") + errLog);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
};


bool Shader::isShaderCompiled(unsigned int shader, int errSize, char *errText) const
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
		glGetShaderInfoLog(shader, errSize, nullptr, errText);
	return success != 0;
}

bool Shader::isProgramLinked(unsigned int program, int errSize, char *errText) const
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
		glGetProgramInfoLog(program, errSize, nullptr, errText);
	return success != 0;
}

void Shader::use()
{
	if (!mShaderProgram && mShaderProgram == sBoundProgram)
		return;
	glUseProgram(mShaderProgram);
	sBoundProgram = mShaderProgram;
}

GLuint Shader::getUniformLocation(std::string const& name) const
{
    GLuint location;
    if (mUniformLocations.count(name) > 0)
        location = mUniformLocations.at(name);
    else
    {
        location = glGetUniformLocation(mShaderProgram, name.c_str());
        mUniformLocations.insert({name, location});
    }
    return location;
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(getUniformLocation(name), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
Shader::~Shader()
{
	glDeleteProgram(mShaderProgram);
};
