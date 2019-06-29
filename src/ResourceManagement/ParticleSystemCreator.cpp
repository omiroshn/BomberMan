/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemCreator.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 11:50:14 by mrassokh          #+#    #+#             */
/*   Updated: 2019/03/02 11:50:15 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/ParticleSystemCreator.hpp"

ParticleSystemCreator & ParticleSystemCreator::getInstance()
{
	static ParticleSystemCreator instance;
	return instance;
}

ParticleSystemCreator::ParticleSystemCreator()
{
	m_createFunctions.push_back(&ParticleSystemCreator::createParticleSystemPoint);
	m_createFunctions.push_back(&ParticleSystemCreator::createParticleSystemQuad);
	m_createFunctions.push_back(&ParticleSystemCreator::createParticleSystemQuad);
	m_createFunctions.push_back(&ParticleSystemCreator::createParticleSystemBrick);
	m_createFunctions.push_back(&ParticleSystemCreator::createParticleSystemQuad);
}

ParticleSystemCreator::~ParticleSystemCreator()
{
	m_createFunctions.clear();
}


psPtr 	ParticleSystemCreator::createParticleSystem(psType type, clePtr clEngine, psInfo const & info) const

{
	return (this->*m_createFunctions[type])(clEngine, info);
}

psPtr 	ParticleSystemCreator::createParticleSystemPoint(clePtr clEngine, psInfo const & info) const
{
	psPtr psPoint = std::shared_ptr<ParticleSystem>(new ParticleSystemPoint());
	initParticleSystem(psPoint, clEngine, info);
	psPoint->setTexture(info.textureName);
	return psPoint;
}

psPtr 	ParticleSystemCreator::createParticleSystemQuad(clePtr clEngine, psInfo const & info) const
{
	psPtr psQuad = std::shared_ptr<ParticleSystem>(new ParticleSystemQuad());
	initParticleSystem(psQuad, clEngine, info);
	psQuad->setTexture(info.textureName);
	return psQuad;
}

psPtr 	ParticleSystemCreator::createParticleSystemBrick(clePtr clEngine, psInfo const & info) const
{
	psPtr psQuad = std::shared_ptr<ParticleSystem>(new ParticleSystemBrick());
	initParticleSystem(psQuad, clEngine, info);
	psQuad->setTexture(info.textureName);
	return psQuad;
}

void 	ParticleSystemCreator::initParticleSystem(psPtr particleSystem, clePtr clEngine, psInfo const & info) const
{
	particleSystem->setCLEngine(clEngine);
	particleSystem->setCLContext();
	particleSystem->setShader(info.shaderName);
	particleSystem->setInitKernelName(info.initName);
	particleSystem->setUpdateKernelName(info.updateName);
	particleSystem->createGLBufers();
	particleSystem->initGLBufers(info.initName);
	particleSystem->setliveTime(info.liveTime);
}
