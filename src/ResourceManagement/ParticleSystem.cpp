/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 13:51:41 by mrassokh          #+#    #+#             */
/*   Updated: 2019/03/01 13:51:44 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/ParticleSystem.hpp"
#include "Game.hpp"

ParticleSystem::ParticleSystem():
						m_initKernelName(""),
						m_particleCount(particleCount),
						m_startTime(std::chrono::high_resolution_clock::now()),
						m_stepTime(std::chrono::high_resolution_clock::now()),
						m_deltaTime(0.0f),
						m_liveTime(LIVE_TIME),
						m_isRunning(false)
{

}

ParticleSystem::ParticleSystem(ParticleSystem const & rhs)
{
	*this = rhs;
}

ParticleSystem & ParticleSystem::operator=(ParticleSystem const &)
{
	return *this;
}

ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_IBO);
}

void 	ParticleSystem::setCLEngine(std::shared_ptr<CLEngine> CLEngine)
{
	if (CLEngine){
		m_CLE = CLEngine;
	} else {
		throw CustomException("CL Engine is not defined!!!");
	}
}

void 	ParticleSystem::setCLContext()
{
	if (m_CLE) {
		m_context = m_CLE->getContext();
	} else {
		throw CustomException("CL Context is not defined!!!");
	}
}
void 	ParticleSystem::setShader(std::string shaderName)
{
	m_shader = RESOURCES.getShader(shaderName);
}

void 		ParticleSystem::setTexture(std::string const & textureName)
{
	m_texture = RESOURCES.getTexture(textureName);
}

void 	ParticleSystem::setParticleCount(int particleCount)
{
	m_particleCount = particleCount;
}

void 	ParticleSystem::setliveTime(float liveTime)
{
	m_liveTime = liveTime;
}

void 		ParticleSystem::addGLBuffer(GLuint & vbo)
{
	cl::BufferGL clBuffer(m_context, CL_MEM_READ_WRITE, vbo);
	m_memory.push_back(clBuffer);
}

void 		ParticleSystem::clearMemoryStack()
{
	m_memory.clear();
}

std::vector<cl::Memory>  &  ParticleSystem::getMemoryStack()
{
	return m_memory;
}

bool			ParticleSystem::isRunning( ) const
{
	return m_isRunning;
}

bool 			ParticleSystem::chekLiveTime() {
	if (m_isRunning)
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		m_deltaTime = Game::mDeltaTime;
		m_stepTime = currentTime;
		m_totalTime += m_deltaTime;
	}
	if (m_totalTime > m_liveTime && m_isRunning)
	{
		stop();
	}
	return m_isRunning;
}

void 			ParticleSystem::start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
	m_stepTime = m_startTime;
	m_isRunning = true;
}

void ParticleSystem::stop()
{
	m_totalTime = 0.0;
	m_isRunning = false;
	initGLBufers(m_initKernelName);
}

std::string const & 		ParticleSystem::getInitKernelName() const
{
	return m_initKernelName;
}

std::string const & 		ParticleSystem::getUpdateKernelName() const
{
	return m_updateKernelName;
}

void 		ParticleSystem::setInitKernelName(std::string const & initName)
{
	m_initKernelName = initName;
}

void 		ParticleSystem::setUpdateKernelName(std::string const & updateName)
{
	m_updateKernelName = updateName;
}
