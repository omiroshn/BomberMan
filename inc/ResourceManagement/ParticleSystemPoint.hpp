

#ifndef PARTICLE_SYSTEM_POINT_HPP
#define PARTICLE_SYSTEM_POINT_HPP

#include "CLEngine.hpp"
#include "Shader.hpp"
#include "CustomException.hpp"
#include "ResourceManager.hpp"
#include "ParticleSystem.hpp"

class ParticleSystemPoint : public ParticleSystem  {
public:
	ParticleSystemPoint();
	virtual ~ParticleSystemPoint();

	virtual void 		createGLBufers();
	virtual void 		setInstanceBuffer();
	virtual void 		initGLBufers(std::string const & initKernelName);
	virtual void 		updateGLBufers(std::string const & updateKernelName);
	virtual void 		drawGLContent(glm::mat4 const & projection, glm::mat4 const & view,
		 								std::vector<glm::mat4> const & transforms);
};

#endif
