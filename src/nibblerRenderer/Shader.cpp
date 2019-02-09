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

#include "Shader.hpp"

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
	catch(std::ifstream::failure &e)
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
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
		glGetProgramInfoLog(program, errSize, nullptr, errText);
	return success != 0;
}

void Shader::use()
{
	glUseProgram(mShaderProgram);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(mShaderProgram, name.c_str()), x, y);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(mShaderProgram, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(mShaderProgram, name.c_str()), x, y, z, w);
}
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
Shader::~Shader()
{
	glDeleteProgram(mShaderProgram);
};

Shader::Shader(Shader const &)
{
};

Shader::Shader()
{
};

Shader &Shader::operator=(Shader const &)
{
	return *this;
};
