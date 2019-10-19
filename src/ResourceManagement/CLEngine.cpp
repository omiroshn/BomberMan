/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLEngine.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 11:11:40 by mrassokh          #+#    #+#             */
/*   Updated: 2019/02/17 11:11:43 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ResourceManagement/CLEngine.hpp"
#if _MSC_VER
#include <windows.h>
#include <windef.h>
#include <pplwin.h>
#endif

CLEngine* CLEngine::m_engine = nullptr;

CLEngine *CLEngine::getInstance()
{
	if (m_engine == nullptr){
		m_engine = new CLEngine();
	};
	return m_engine->m_isInit ? m_engine : nullptr;
}

CLEngine::CLEngine() : m_isInit(false)
{
	try {
		init();
		m_isInit = true;
	} catch (std::ifstream::failure &e) {
		clear();
		std::cout << "CL Engine initialisation failed: " << e.what()
		<< std::endl;
	}
}

CLEngine::~CLEngine()
{
	clear();
}

void 	CLEngine::defineDevice()
{
	cl::Platform::get(&m_allPlatforms);
	if (!m_allPlatforms.size())
		throw CustomException("CL Platforms is not found");

	m_usedPlatform = m_allPlatforms.front();
	m_usedPlatform.getDevices(CL_DEVICE_TYPE_GPU, &m_allDevices);
	if (m_allDevices.size() < 1)
		throw CustomException("Graphic device is not found");

	m_usedDevice = m_allDevices[0];
	auto vendor = m_usedDevice.getInfo<CL_DEVICE_VENDOR>();
	auto version = m_usedDevice.getInfo<CL_DEVICE_VERSION>();
}

void 	CLEngine::defineContext()
{
	#if defined(_WIN32)
		WINGDIAPI HGLRC WINAPI wglGetCurrentContext(VOID);
		WINGDIAPI HDC   WINAPI wglGetCurrentDC(VOID);

		// Windows
		cl_context_properties conextProperties[] =
		 {
		  CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		  CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		  0
		};
		m_context = cl::Context(m_usedDevice, conextProperties);

	#elif defined(__APPLE__)

		// OS X
		CGLContextObj     kCGLContext     = CGLGetCurrentContext();
		CGLShareGroupObj  kCGLShareGroup  = CGLGetShareGroup(kCGLContext);

		cl_context_properties conextProperties[] =
		{
		  CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		  (cl_context_properties) kCGLShareGroup,
		  0
		};
		m_context = cl::Context(m_usedDevice, conextProperties);
	#else

		// Linux
		  cl_context_properties conextProperties[] = {
		  CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
		  CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		  CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
		  0
		};
		m_context = cl::Context(m_usedDevice, conextProperties);
	#endif

	m_commandQueue = cl::CommandQueue(m_context, m_usedDevice);
}

void	CLEngine::addProgramSource(std::string const & fileName,
									std::string const & sourceName)
{
	std::ifstream file(fileName);
	if (!file)
		throw CustomException(fileName + " is not opened");

	std::string sourceStr(std::istreambuf_iterator<char>(file),
									(std::istreambuf_iterator<char>()));

	auto source = m_programSources.find(sourceName);
	if (source == m_programSources.end()) {
		cl::Program::Sources sources;
		sources.push_back(std::make_pair(strdup(sourceStr.c_str()), sourceStr.length() + 1)); // we use strdup here. it calls malloc. but who will free that memory?
		m_programSources.emplace(std::make_pair(sourceName, sources));
	} else {
		source->second.push_back(std::make_pair(strdup(sourceStr.c_str()), sourceStr.length() + 1));
	}
}

void 	CLEngine::addProgramFromSource(std::string const & sourceName, std::string const & programName)
{
	if (!m_isInit) {
		throw CustomException("CL Engine is not initialised");
	}
	auto source = m_programSources.find(sourceName);

	if (source != m_programSources.end()) {
	   cl::Program program(m_context, source->second);
	   program.build({m_usedDevice});
	   m_programs.emplace(std::make_pair(programName, program));
   	} else {
		throw CustomException(sourceName + " is not found");
   	}
}

void 	CLEngine::addKernel(std::string const & programName,
							std::string const & kernelName, std::string const & funcName)
{
	if (!m_isInit) {
		throw CustomException("CL Engine is not initialised");
	}

	auto program = m_programs.find(programName);

	if (program != m_programs.end()) {
	   cl::Kernel kernel(program->second, funcName.c_str());
	   m_kernels.emplace(std::make_pair(kernelName, kernel));
   	} else {
		throw CustomException(programName + " is not found");
   	}
}

cl::CommandQueue const & 	CLEngine::getCommandQueue()
{
	if (!m_isInit) {
		throw CustomException("CL Engine is not initialised");
	}
	return m_commandQueue;
}

void 			CLEngine::getKernel(std::string const & kernelName, cl::Kernel & kernelOut)
{
	if (!m_isInit) {
		throw CustomException("CL Engine is not initialised");
	}

	auto kernel = m_kernels.find(kernelName);

	if (kernel != m_kernels.end()) {
	   kernelOut = kernel->second;
   	} else {
		throw CustomException(kernelName + " is not found");
	}
}

cl::Context const &		CLEngine::getContext() const
{
	if (!m_isInit) {
		throw CustomException("CL Engine is not initialised");
	}

	return m_context;
}

void 		CLEngine::init()
{
	defineDevice();
	defineContext();
}

void 		CLEngine::clear()
{
	m_allPlatforms.clear();
	m_allDevices.clear();
	m_programs.clear();
	m_programSources.clear();
	m_kernels.clear();
}
