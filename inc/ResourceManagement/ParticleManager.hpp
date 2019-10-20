/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 12:12:09 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/25 12:12:11 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "CLEngine.hpp"
// #include "Shader.hpp"


#ifndef PARTICLE_MANAGER_HPP
#define PARTICLE_MANAGER_HPP
#include "ResourceManagement/ParticleSystem.hpp"
#include "ResourceManagement/ParticleSystemCreator.hpp"

typedef std::map<std::string, psPtr> ParticleSystemMap;
typedef std::map<psType, psInfo> ParticleInfoMap;
typedef std::map<std::string,std::vector<glm::mat4>> ParticleSystemTransMap;

class ParticleManager{
public:
	ParticleManager();
	ParticleManager(ParticleManager const & rhs) = delete;
	ParticleManager & operator=(ParticleManager const & rhs) = delete;
	~ParticleManager();

	void 						init();
	void 						startDrawPS(std::string const & name, std::vector<glm::mat4> const & transforms);
	void 						draw(glm::mat4 const & projection, glm::mat4 const & view);
	void 						update();
	void 						clear();


private:
	std::shared_ptr<CLEngine> 	m_CLE;
	ParticleSystemMap			m_particleSystemMap;
	ParticleInfoMap				m_particleInfoMap;
	ParticleSystemTransMap		m_transformsMap;


	void 						initCLEngine();
	void 						addParticleSystem(std::string const & name, psType type);
	void 						initDefaultParticleInfo(psType type, std::string const & initName, std::string const & updateName,
															std::string const & shaderName, std::string const & textureName, float const liveTime);
};

#endif
