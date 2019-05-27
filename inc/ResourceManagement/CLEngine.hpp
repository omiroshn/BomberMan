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
#include "GL/glew.h"
#include <CL/cl.h>
#include <CL/cl.hpp>
//#include <GL/OpenGL.h>
#endif
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>
#include "CustomException.hpp"

typedef std::map<std::string, cl::Program::Sources> programSources;
typedef std::map<std::string, cl::Program> programs;
typedef std::map<std::string, cl::Kernel> kernels;


class  CLEngine {
public:
	static CLEngine   			*getInstance();
	~CLEngine();

	void 						defineDevice();
	void 						defineContext();
	void 						addProgramSource(std::string const & fileName, std::string const & sourceName);
	void						addProgramFromSource(std::string const & sourceName, std::string const & programName);
	void 						addKernel(std::string const & programName, std::string const & kernelName,
													std::string const & funcName);
	cl::CommandQueue const & 	getCommandQueue();
	void 						getKernel(std::string const & kernelName, cl::Kernel & kernelOut);
	cl::Context const &			getContext() const;
	cl::Device const & 			getDevice() const {return m_usedDevice;};
	void 						init();
	void 						clear();



private:
	CLEngine();
	CLEngine(CLEngine const & rhs) = delete;
	CLEngine & operator = (CLEngine const & rhs) = delete;


	static CLEngine 			*m_engine;

	std::vector<cl::Platform> 	m_allPlatforms;
	std::vector<cl::Device>		m_allDevices;
	cl::Platform				m_usedPlatform;
	cl::Device  				m_usedDevice;

	cl::Context 				m_context;
	cl::CommandQueue 			m_commandQueue;

	programSources 				m_programSources;
	programs					m_programs;
	kernels						m_kernels;
	bool 						m_isInit;
};
#endif
