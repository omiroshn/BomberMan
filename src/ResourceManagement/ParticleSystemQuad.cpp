/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemQuad.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 10:51:25 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/28 10:51:28 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/ParticleSystemQuad.hpp"

ParticleSystemQuad::ParticleSystemQuad()
{
	setParticleCount(500);
}

ParticleSystemQuad::~ParticleSystemQuad()
{

}

void 		ParticleSystemQuad::createGLBufers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadParticle) * m_particleCount * 6, nullptr, GL_STREAM_DRAW);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadParticle), (GLvoid*)0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadParticle), (GLvoid*)(4* sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setInstanceBuffer();
}

void 				ParticleSystemQuad::setInstanceBuffer()
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

void 			ParticleSystemQuad::initGLBufers(std::string const & initKernelName)
{
	glFinish();
	cl::Kernel kernel;

	m_CLE->getKernel(initKernelName, kernel);
	clearMemoryStack();
	addGLBuffer(m_VBO);

	auto commandQueue = m_CLE->getCommandQueue();
	commandQueue.enqueueAcquireGLObjects(&m_memory);

	kernel.setArg(0, m_memory.front());
	kernel.setArg(1, m_particleCount);

	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_particleCount * 6), cl::NullRange);
	//commandQueue.finish();
	commandQueue.enqueueReleaseGLObjects(&m_memory);
	//cl::finish();
}

void 			ParticleSystemQuad::updateGLBufers(std::string const & updateKernelName)
{
	glFinish();

	cl::Kernel kernel;
	m_CLE->getKernel(updateKernelName, kernel);

	auto commandQueue = m_CLE->getCommandQueue();
 	commandQueue.enqueueAcquireGLObjects(&m_memory);
	kernel.setArg(0, m_memory.front());
	kernel.setArg(1, m_deltaTime);
	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_particleCount * 6), cl::NullRange);
	commandQueue.enqueueReleaseGLObjects(&m_memory);
	//commandQueue.finish();
	//cl::finish();
}

void 			ParticleSystemQuad::drawGLContent(glm::mat4 const & projection, glm::mat4  const & view, std::vector<glm::mat4> const & transforms)
{
	if (!m_shader)
		throw CustomException("Shader for particle system is not defined!");
	if (!m_texture)
		throw CustomException("Texture for ParticleSystemQuad is not defined!");
	m_shader->use();
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("view", view);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	m_texture->bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), &transforms[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_particleCount * 6, transforms.size());
    glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
