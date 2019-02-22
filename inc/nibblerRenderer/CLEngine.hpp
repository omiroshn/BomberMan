/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLEngine.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 11:13:19 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/17 11:13:21 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_ENGINE_HPP
#define CL_ENGINE_HPP

#ifdef __APPLE__
#include "GL/glew.h"
#include <OpenGL/OpenGL.h>
#include <OpenCL/opencl.h>
#include "cl.hpp"
#else
#include <CL/cl.h>
#include <CL/cl.hpp>
//#include <GL/OpenGL.h>
#endif
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>

typedef std::map<std::string, cl::Program::Sources> programSources;
typedef std::map<std::string, cl::Program> programs;
typedef std::map<std::string, cl::Kernel> kernels;

class CLEngine {
public:
	void 						defineDevice();
	void 						defineContext();
	
	bool 						addProgramSource(std::string const & fileName,
												std::string const & sourceName);
	bool						addProgramFromSource(std::string const & sourceName,
													std::string const & programName);
	bool 						addKernel(std::string const & programName,
											std::string const & kernelName,
													std::string const & funcName);
	void 						addGLBuffer(GLuint & vbo);
	void 						clearMemoryStack();
	std::vector<cl::Memory> &   getMemoryStack();

	cl::CommandQueue const & 	getCommandQueue();
	void 						getKernel(std::string const & kernelName, cl::Kernel & kernelOut);
	cl::Context const &			getContext() const;
	cl::Device const & 			getDevice() const {return m_usedDevice;};

private:
	std::vector<cl::Platform> 	m_allPlatforms;
	std::vector<cl::Device>		m_allDevices;
	cl::Platform				m_usedPlatform;
	cl::Device  				m_usedDevice;

	cl::Context 				m_context;
	cl::CommandQueue 			m_commandQueue;

	programSources 				m_programSources;
	programs					m_programs;
	kernels						m_kernels;

	std::vector<cl::Memory> 	m_memory;

};
#endif
