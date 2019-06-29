/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemBrick.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 10:49:39 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/28 10:49:41 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef PARTICLE_SYSTEM_BRICK_HPP
#define PARTICLE_SYSTEM_BRICK_HPP

#include "CLEngine.hpp"
#include "Shader.hpp"
#include "CustomException.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Texture.hpp"
#include "ParticleSystem.hpp"

class ParticleSystemBrick : public ParticleSystem {
public:
	ParticleSystemBrick();
	virtual ~ParticleSystemBrick();

	virtual void 					createGLBufers();
	virtual void 					setInstanceBuffer();
	virtual void 					initGLBufers(std::string const & initKernelName);
	virtual void 					updateGLBufers(std::string const & updateKernelName);
	virtual void 					drawGLContent(glm::mat4 const & projection,
		 											glm::mat4 const & view,
														std::vector<glm::mat4> const & transforms);
};

#endif
