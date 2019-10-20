/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 13:51:59 by mrassokh          #+#    #+#             */
/*   Updated: 2019/03/01 13:52:02 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "Shader.hpp"
#include "CLEngine.hpp"
#include "CustomException.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include <chrono>

# define LIVE_TIME 1.6f
# define LIVE_TIME_CLOUD 2000000.0f

struct Particle
{
	cl_float4	position;
};

struct QuadParticle : Particle
{
 	cl_float2	uv;
};


struct Quad
{
 	QuadParticle particle[6];
};

struct Brick
{
 	Quad quad[6];
};


const int particleCount = 1000000;
typedef std::chrono::time_point<std::chrono::high_resolution_clock>	t_point;

class ParticleSystem {
public:
	ParticleSystem();
	ParticleSystem(ParticleSystem const & rhs);
	ParticleSystem & operator = (ParticleSystem const & rhs);
	virtual ~ParticleSystem();

	std::string const & 		getInitKernelName() const;
	std::string const & 		getUpdateKernelName() const;
	void 						setInitKernelName(std::string const & initName);
	void 						setUpdateKernelName(std::string const & updateName);

	void 						setCLEngine(std::shared_ptr<CLEngine>);
	void 						setCLContext();
	void 						setParticleCount(int particleCount);
	void 						setShader(std::string shaderName);
	void 						setliveTime(float liveTime);

	void 						addGLBuffer(GLuint & vbo);
	void 						clearMemoryStack();
	std::vector<cl::Memory>  &  getMemoryStack();

	void 						start();
	void 						stop();
	bool						isRunning( ) const;
	bool 						chekLiveTime();


	virtual void				createGLBufers() = 0;
	virtual void 				setInstanceBuffer() = 0;
	virtual void 				setTexture(std::string const & textureName);
	virtual void 				initGLBufers(std::string const & initKernelName) = 0;
	virtual void 				updateGLBufers(std::string const & updateKernelName) = 0;
	virtual void				drawGLContent(glm::mat4 const & projection, glm::mat4  const & view, std::vector<glm::mat4> const & transforms) = 0;

protected:
	std::shared_ptr<CLEngine> 	m_CLE;
	std::shared_ptr<Shader> 	m_shader;
	std::string					m_initKernelName;
	std::string					m_updateKernelName;
	cl::Context 				m_context;
	std::vector<cl::Memory> 	m_memory;
	std::shared_ptr<Texture>  	m_texture;

	GLuint 						m_VBO;
	GLuint 						m_VAO;
	GLuint						m_IBO;

	int							m_particleCount;
	t_point						m_startTime;
	t_point						m_stepTime;
	float						m_deltaTime;
	float						m_totalTime = 0.f; // this in some cases is not initialized. no idea why.
	float						m_liveTime;
	bool						m_isRunning;
};

#endif
