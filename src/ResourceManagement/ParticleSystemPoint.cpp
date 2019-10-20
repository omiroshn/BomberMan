#include "ResourceManagement/ParticleSystemPoint.hpp"
#include "ResourceManagement/Texture.hpp"

ParticleSystemPoint::ParticleSystemPoint()
{
	setParticleCount(80);
}

ParticleSystemPoint::~ParticleSystemPoint()
{

}

void 		ParticleSystemPoint::createGLBufers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_particleCount, nullptr, GL_STREAM_DRAW);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(4* sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setInstanceBuffer();
}

void 				ParticleSystemPoint::setInstanceBuffer()
{
	glBindVertexArray(m_VAO);
    glGenBuffers(1, &m_IBO);


    glBindBuffer(GL_ARRAY_BUFFER, m_IBO);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(0 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void 			ParticleSystemPoint::initGLBufers(std::string const & initKernelName)
{
	cl::Kernel kernel;

	clearMemoryStack();
	addGLBuffer(m_VBO);

	m_CLE->getKernel(initKernelName, kernel);
	auto commandQueue = m_CLE->getCommandQueue();
	commandQueue.enqueueAcquireGLObjects(&m_memory);

	kernel.setArg(0, m_memory.front());
	kernel.setArg(1, m_particleCount);

	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_particleCount), cl::NullRange);
	commandQueue.enqueueReleaseGLObjects(&m_memory);
}

void 			ParticleSystemPoint::updateGLBufers(std::string const & updateKernelName)
{
	cl::Kernel kernel;
	m_CLE->getKernel(updateKernelName, kernel);

	auto commandQueue = m_CLE->getCommandQueue();
 	commandQueue.enqueueAcquireGLObjects(&m_memory);
	kernel.setArg(0, m_memory.front());
	kernel.setArg(1, m_deltaTime);

	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_particleCount), cl::NullRange);
	commandQueue.enqueueReleaseGLObjects(&m_memory);
}

void 			ParticleSystemPoint::drawGLContent(glm::mat4 const & projection, glm::mat4  const & view, std::vector<glm::mat4> const & transforms)
{
	if (!m_shader)
		throw CustomException("Shader for particle system is not defined!");

	m_shader->use();
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("view", view);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), &transforms[0], GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	m_texture->bind();

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_POINTS, 0, m_particleCount, GLsizei(transforms.size()));
	glDisable(GL_BLEND);
}
