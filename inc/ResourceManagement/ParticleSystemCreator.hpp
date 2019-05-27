/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemCreator.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 11:48:05 by mrassokh          #+#    #+#             */
/*   Updated: 2019/03/02 11:48:07 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLE_SYSTEM_CREATOR_HPP
#define PARTICLE_SYSTEM_CREATOR_HPP

#include "CLEngine.hpp"
#include "ParticleSystemPoint.hpp"
#include "ParticleSystemQuad.hpp"
#include "ParticleSystemBrick.hpp"

#define PARTICLE_CREATOR ParticleSystemCreator::getInstance()

typedef enum 	ParticleSystemType
{
	POINTCL,
  	QUAD,
	QUAD_TMAP,
	BRICK,
	CLOUD
}				psType;

typedef std::shared_ptr<ParticleSystem> psPtr;
typedef std::shared_ptr<CLEngine> 		clePtr;

typedef struct 				ParticleSystemInfo
{
	std::string   		initName;
	std::string   		updateName;
	std::string   		shaderName;
	std::string  		textureName;
	float				liveTime;
}							psInfo;

class ParticleSystemCreator {
public:
	static ParticleSystemCreator & getInstance();

	psPtr 	createParticleSystem(psType type, clePtr clEngine, psInfo const & info) const;

private:
	ParticleSystemCreator();
	ParticleSystemCreator(ParticleSystemCreator const & ) = delete;
	ParticleSystemCreator & operator = (ParticleSystemCreator const & ) = delete;
	virtual ~ParticleSystemCreator();

	psPtr 	createParticleSystemPoint(clePtr clEngine, psInfo const & info) const;
	psPtr 	createParticleSystemQuad(clePtr clEngine, psInfo const & info) const;
	psPtr 	createParticleSystemBrick(clePtr clEngine, psInfo const & info) const;
	void 	initParticleSystem(psPtr particleSystem, clePtr clEngine, psInfo const & info) const;

	std::vector<psPtr (ParticleSystemCreator::*)
					(clePtr clEngine, psInfo const & info) const>	m_createFunctions;
};
#endif
