/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 12:11:44 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/25 12:11:47 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/ParticleManager.hpp"

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
		initDefaultParticleInfo(POINT,"initSphereKernel", "updateSphereKernel","sprite_p", "", LIVE_TIME);
		initDefaultParticleInfo(QUAD,"initSphereKernelQuad", "updateSphereKernelQuad",
														"sprite_quad", "flame-fire", LIVE_TIME);
		initDefaultParticleInfo(QUAD_TMAP,"initSphereKernelQuadTMap", "updateSphereKernelQuadTMap",
														"sprite_quad", "explosion_tmap_2", LIVE_TIME);
		initDefaultParticleInfo(BRICK,"initBrickBlockKernel", "updateBrickBlockKernel",
														"sprite_quad", "container", LIVE_TIME);
		initDefaultParticleInfo(CLOUD,"initCloudKernel", "updateCloudKernel",
																"sprite_quad_cloud", "cloud_trans", LIVE_TIME_CLOUD);
		addParticleSystem("pointSphereBomb", POINT);
		addParticleSystem("quadSphereBomb", QUAD);
		addParticleSystem("quadSphereBombTMap", QUAD_TMAP);
		addParticleSystem("BrickBlock", BRICK);
		addParticleSystem("Cloud", CLOUD);
	} catch (CustomException &ex) {
		std::cout << ex.what() << std::endl;
		exit(42);
	}
}

void 		ParticleManager::initCLEngine()
{
	try {
		//init CLEngine
		m_CLE->defineDevice();
		m_CLE->defineContext();

		//set sources prorams and kernels for initialise Sphere by Points
		m_CLE->addProgramSource("../Assets/kernel/init_sphere_by_points.cl", "initSpherePoints");
		m_CLE->addProgramFromSource("initSpherePoints", "initSphereProgram");
		m_CLE->addKernel("initSphereProgram", "initSphereKernel","initialize_sphere");
		//set sources prorams and kernels for update Sphere by Points
		m_CLE->addProgramSource("../Assets/kernel/update_sphere_by_points.cl", "updateSpherePoints");
		m_CLE->addProgramFromSource("updateSpherePoints", "updateSphereProgram");
		m_CLE->addKernel("updateSphereProgram", "updateSphereKernel","update_sphere");

		//set sources prorams and kernels for initialise Sphere by Quads
		m_CLE->addProgramSource("../Assets/kernel/init_sphere_by_quads.cl", "initSphereQuads");
		m_CLE->addProgramFromSource("initSphereQuads", "initSphereProgramQuad");
		m_CLE->addKernel("initSphereProgramQuad", "initSphereKernelQuad","initialize_sphere");
		//set sources prorams and kernels for update Sphere by Quads
		m_CLE->addProgramSource("../Assets/kernel/update_sphere_by_quads.cl", "updateSphereQuads");
		m_CLE->addProgramFromSource("updateSphereQuads", "updateSphereProgramQuad");
		m_CLE->addKernel("updateSphereProgramQuad", "updateSphereKernelQuad","update_sphere");

		//set sources prorams and kernels for initialise Sphere by Quads with texture map
		m_CLE->addProgramSource("../Assets/kernel/init_sphere_by_quads_tmap.cl", "initSphereQuadsTMap");
		m_CLE->addProgramFromSource("initSphereQuadsTMap", "initSphereProgramQuadTMap");
		m_CLE->addKernel("initSphereProgramQuadTMap", "initSphereKernelQuadTMap","initialize_sphere");
		//set sources prorams and kernels for update Sphere by Quads with texture map
		m_CLE->addProgramSource("../Assets/kernel/update_sphere_by_quads_tmap.cl", "updateSphereQuadsTMap");
		m_CLE->addProgramFromSource("updateSphereQuadsTMap", "updateSphereProgramQuadTMap");
		m_CLE->addKernel("updateSphereProgramQuadTMap", "updateSphereKernelQuadTMap","update_sphere");

		//set sources prorams and kernels for initialise Brick Block
		m_CLE->addProgramSource("../Assets/kernel/init_brick_block.cl", "initBrickBlock");
		m_CLE->addProgramFromSource("initBrickBlock", "initBrickBlockProgram");
		m_CLE->addKernel("initBrickBlockProgram", "initBrickBlockKernel","initialize_brick_block");
		//set sources prorams and kernels for update  Brick Block
		m_CLE->addProgramSource("../Assets/kernel/update_brick_block.cl", "updateBrickBlock");
		m_CLE->addProgramFromSource("updateBrickBlock", "updateBrickBlockProgram");
		m_CLE->addKernel("updateBrickBlockProgram", "updateBrickBlockKernel","update_brick_block");

		//set sources prorams and kernels for initialise Cloud
		m_CLE->addProgramSource("../Assets/kernel/init_cloud.cl", "initCloud");
		m_CLE->addProgramFromSource("initCloud", "initCloudProgram");
		m_CLE->addKernel("initCloudProgram", "initCloudKernel","initialize_cloud");
		//set sources prorams and kernels for update  Cloud
		m_CLE->addProgramSource("../Assets/kernel/update_cloud.cl", "updateCloud");
		m_CLE->addProgramFromSource("updateCloud", "updateCloudProgram");
		m_CLE->addKernel("updateCloudProgram", "updateCloudKernel","update_cloud");
	} catch (CustomException &ex) {
		std::cout << ex.what() << std::endl;
		exit(42);
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
	//auto start = std::chrono::high_resolution_clock::now();
	for (auto & element : m_particleSystemMap) {
		//std::cout << "try draw element: " << element.first<<std::endl;
		auto & ps = element.second;
		if (ps->chekLiveTime()){
			// std::cout << "draw element: " << element.first<<std::endl;
			// auto start1 = std::chrono::high_resolution_clock::now();
			ps->updateGLBufers(ps->getUpdateKernelName());
			// auto end1 = std::chrono::high_resolution_clock::now();
		   // std::chrono::duration<double> diff1 = end1-start1;
	  //std::cout << "updateGLContent: "<<diff1.count()<<std::endl;
			auto trans = m_transformsMap.find(element.first);
			if (trans != m_transformsMap.end()){
				auto start = std::chrono::high_resolution_clock::now();
				ps->drawGLContent(projection, view, trans->second);
			// 	 auto end = std::chrono::high_resolution_clock::now();
			// 	std::chrono::duration<double> diff = end-start;
		   // std::cout << "drawGLContent: "<<diff.count()<<std::endl;
			} else {
				throw CustomException(element.first + " ParticleSystem transforms is not found");
			}
		}
	}

	// auto end = std::chrono::high_resolution_clock::now();
	// std::chrono::duration<double> diff = end-start;
    // std::cout << "draw function: " << diff.count()<<std::endl;
}
