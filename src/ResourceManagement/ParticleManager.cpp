/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 12:11:44 by mrassokh          #+#    #+#             */
/*   Updated: 2019/06/22 14:45:30 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/ParticleManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include <Game.hpp>

ParticleManager::ParticleManager()
{
	m_CLE = std::shared_ptr<CLEngine>(CLEngine::getInstance());
}


ParticleManager::~ParticleManager()
{
	m_particleSystemMap.clear();
}

void 		ParticleManager::init()
{
	try {
		initCLEngine();
		initDefaultParticleInfo(POINTCL,	"initSphereKernel", 		"updateSphereKernel", 			"sprite_p", 		 "flame-fire", LIVE_TIME);
		initDefaultParticleInfo(BRICK,		"initBrickBlockKernel", 	"updateBrickBlockKernel", 		"sprite_quad_brick", "container", LIVE_TIME);
		addParticleSystem("pointSphereBomb", POINTCL);
		addParticleSystem("pointSphereBomb2", POINTCL);
		addParticleSystem("BrickBlock", BRICK);
		addParticleSystem("BrickBlock2", BRICK);
	} catch (CustomException &ex) {
		std::cout << ex.what() << std::endl;
		Game::get()->requestExit();
	}
}

void ParticleManager::clear()
{
	for (auto & element : m_particleSystemMap)
		element.second->stop();
	m_transformsMap.clear();
}

void 		ParticleManager::initCLEngine()
{

	try {
        auto const& binFolder = RESOURCES.getBinFolder();
		//set sources prorams and kernels for initialise Sphere by Points
		m_CLE->addProgramSource(binFolder + "/kernel/init_sphere_by_points.cl", "initSpherePoints");
		m_CLE->addProgramFromSource("initSpherePoints", "initSphereProgram");
		m_CLE->addKernel("initSphereProgram", "initSphereKernel","initialize_sphere");
		//set sources prorams and kernels for update Sphere by Points
		m_CLE->addProgramSource(binFolder + "/kernel/update_sphere_by_points.cl", "updateSpherePoints");
		m_CLE->addProgramFromSource("updateSpherePoints", "updateSphereProgram");
		m_CLE->addKernel("updateSphereProgram", "updateSphereKernel","update_sphere");

		//set sources prorams and kernels for initialise Brick Block
		m_CLE->addProgramSource(binFolder + "/kernel/init_brick_block.cl", "initBrickBlock");
		m_CLE->addProgramFromSource("initBrickBlock", "initBrickBlockProgram");
		m_CLE->addKernel("initBrickBlockProgram", "initBrickBlockKernel","initialize_brick_block");
		//set sources prorams and kernels for update  Brick Block
		m_CLE->addProgramSource(binFolder + "/kernel/update_brick_block.cl", "updateBrickBlock");
		m_CLE->addProgramFromSource("updateBrickBlock", "updateBrickBlockProgram");
		m_CLE->addKernel("updateBrickBlockProgram", "updateBrickBlockKernel","update_brick_block");
	} catch (CustomException &ex) {
		std::cout << ex.what() << std::endl;
		Game::get()->requestExit();
	}
}


void 			ParticleManager::initDefaultParticleInfo(psType type, std::string const & initName, std::string const & updateName,
														std::string const & shaderName, std::string const & textureName, float const liveTime)
{
	psInfo info;

	info.initName = initName;
	info.updateName = updateName;
	info.shaderName = shaderName;
	info.textureName = textureName;
	info.liveTime = liveTime;

	m_particleInfoMap.emplace(type, info);
}

void 			ParticleManager::addParticleSystem(std::string const & name, psType type)
{
	auto info = m_particleInfoMap.find(type);
	if (info == m_particleInfoMap.end())
		throw CustomException("ParticleSystem info is not found");

	psPtr particleSystem = PARTICLE_CREATOR.createParticleSystem(type, m_CLE, info->second);
	m_particleSystemMap.emplace(name, particleSystem);
}

void 			ParticleManager::startDrawPS(std::string const & name, std::vector<glm::mat4> const & transforms)
{
	auto ps = m_particleSystemMap.find(name);
	if (ps == m_particleSystemMap.end())
		throw CustomException(name + " ParticleSystem  is not found");
	if (ps->second->isRunning())
		return;

	auto trans = m_transformsMap.find(name);
	if (trans != m_transformsMap.end())
		m_transformsMap.erase(trans);
	m_transformsMap.emplace(name,transforms);
	ps->second->start();
}

void 			ParticleManager::draw(glm::mat4 const & projection, glm::mat4 const & view)
{
	for (auto & element : m_particleSystemMap) {
		auto & ps = element.second;
		if (ps->isRunning())
		{
			auto trans = m_transformsMap.find(element.first);
			if (trans != m_transformsMap.end()){
				ps->drawGLContent(projection, view, trans->second);
			} else {
				throw CustomException(element.first + " ParticleSystem transforms is not found");
			}
		}
	}
}

void ParticleManager::update()
{
	for (auto& element : m_particleSystemMap) {
		auto& ps = element.second;
		if (ps->chekLiveTime())
			ps->updateGLBufers(ps->getUpdateKernelName());
	}
}
