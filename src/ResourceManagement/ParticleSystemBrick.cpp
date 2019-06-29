/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemBrick.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 10:51:25 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/28 10:51:28 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/ParticleSystemBrick.hpp"

ParticleSystemBrick::ParticleSystemBrick()
{
	setParticleCount(48);
}

ParticleSystemBrick::~ParticleSystemBrick()
{

}

void 		ParticleSystemBrick::createGLBufers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Brick) * m_particleCount, nullptr, GL_STREAM_DRAW);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadParticle), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadParticle), (GLvoid*)(4* sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadParticle), (GLvoid*)(8* sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setInstanceBuffer();
}

void 				ParticleSystemBrick::setInstanceBuffer()
{
	glBindVertexArray(m_VAO);
    glGenBuffers(1, &m_IBO);


    glBindBuffer(GL_ARRAY_BUFFER, m_IBO);
	glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(0 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void 			ParticleSystemBrick::initGLBufers(std::string const & initKernelName)
{
	//glFinish();
	cl::Kernel kernel;

	m_CLE->getKernel(initKernelName, kernel);
	clearMemoryStack();
	addGLBuffer(m_VBO);

	auto commandQueue = m_CLE->getCommandQueue();
	commandQueue.enqueueAcquireGLObjects(&m_memory);

	kernel.setArg(0, m_memory.front());

	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_particleCount), cl::NullRange);
	//commandQueue.finish();
	commandQueue.enqueueReleaseGLObjects(&m_memory);
	//cl::finish();
}

void 			ParticleSystemBrick::updateGLBufers(std::string const & updateKernelName)
{
	glFinish();

	cl::Kernel kernel;
	m_CLE->getKernel(updateKernelName, kernel);

	auto commandQueue = m_CLE->getCommandQueue();
 	commandQueue.enqueueAcquireGLObjects(&m_memory);
	kernel.setArg(0, m_memory.front());
	kernel.setArg(1, m_deltaTime);
	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_particleCount), cl::NullRange);
	commandQueue.enqueueReleaseGLObjects(&m_memory);
	//commandQueue.finish();
	//cl::finish();
}

void 			ParticleSystemBrick::drawGLContent(glm::mat4 const & projection, glm::mat4  const & view, std::vector<glm::mat4> const & transforms)
{
	if (!m_shader)
		throw CustomException("Shader for particle system is not defined!");
	if (!m_texture)
		throw CustomException("Texture for Brick system is not defined!");

	m_shader->use();
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("view", view);
	//glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	m_texture->bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), &transforms[0], GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_particleCount * 36, transforms.size());
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glFinish();
}
