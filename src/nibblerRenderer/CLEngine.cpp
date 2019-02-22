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

# include "CLEngine.hpp"

void 	CLEngine::defineDevice()
{
	cl::Platform::get(&m_allPlatforms);
	assert(m_allPlatforms.size()>0);

	m_usedPlatform = m_allPlatforms.front();
	m_usedPlatform.getDevices(CL_DEVICE_TYPE_ALL, &m_allDevices);
	assert(m_allDevices.size()>0);
	std::cout << "Using platform: "<< m_usedPlatform.getInfo<CL_PLATFORM_NAME>()<<"\n";

	m_usedDevice = m_allDevices[1];
	auto vendor = m_usedDevice.getInfo<CL_DEVICE_VENDOR>();
	auto version = m_usedDevice.getInfo<CL_DEVICE_VERSION>();
	std::cout<< "Using device: "<< m_usedDevice.getInfo<CL_DEVICE_NAME>() << std::endl;
	std::cout<< "Using device:" << vendor << "with version" << version << std::endl;
}

void 	CLEngine::defineContext()
{
	#if defined(_WIN32)

		// Windows
		cl_context_properties conextProperties[] =
		 {
		  CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		  CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		  CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
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

bool	CLEngine::addProgramSource(std::string const & fileName,
									std::string const & sourceName)
{
	std::ifstream file(fileName);
	if (!file)
		return false;
	std::string sourceStr(std::istreambuf_iterator<char>(file),
									(std::istreambuf_iterator<char>()));

	auto source = m_programSources.find(sourceName);
	if (source == m_programSources.end()) {
		std::cout << "print new program source for empty " << sourceName << std::endl;
		std::cout << sourceStr.c_str() << std::endl;

	cl::Program::Sources sources;
	sources.push_back(std::make_pair(strdup(sourceStr.c_str()), sourceStr.length() + 1));
	m_programSources.emplace(std::make_pair(sourceName, sources));
	} else {
		std::cout << "print new program source for existing " << sourceName << std::endl;
		std::cout << sourceStr.c_str() << std::endl;
		source->second.push_back(std::make_pair(strdup(sourceStr.c_str()), sourceStr.length() + 1));
	}
	return true;
}

bool 	CLEngine::addProgramFromSource(std::string const & sourceName, std::string const & programName)
{
	auto source = m_programSources.find(sourceName);

	if (source != m_programSources.end()) {
	   std::cout << "Found _programSources" << source->first <<'\n';
	   for (auto s : source->second){
		    std::cout << "source str with size " << s.second << " : " << s.first <<'\n';
	   }
	   cl::Program program(m_context, source->second);
	   program.build({m_usedDevice});
	   m_programs.emplace(std::make_pair(programName, program));
	   return true;

   	} else {
	   std::cout << sourceName << " not found\n";
	   return false;
   	}
}

bool 	CLEngine::addKernel(std::string const & programName,
							std::string const & kernelName, std::string const & funcName)
{
	auto program = m_programs.find(programName);

	if (program != m_programs.end()) {
	   std::cout << "Found _program" << program->first <<'\n';
	   cl::Kernel kernel(program->second, funcName.c_str());
	   m_kernels.emplace(std::make_pair(kernelName, kernel));
	   return true;
   	} else {
	   std::cout << programName << " not found\n";
	   return false;
   	}
}

void 	CLEngine::addGLBuffer(GLuint & vbo)
{
	cl::BufferGL clBuffer(m_context, CL_MEM_READ_WRITE, vbo);
	m_memory.push_back(clBuffer);
}

void 	CLEngine::clearMemoryStack()
{
	m_memory.clear();
}

std::vector<cl::Memory> &   CLEngine::getMemoryStack()
{
	return m_memory;
}

cl::CommandQueue const & 	CLEngine::getCommandQueue()
{
	return m_commandQueue;
}

void 			CLEngine::getKernel(std::string const & kernelName, cl::Kernel & kernelOut)
{
	auto kernel = m_kernels.find(kernelName);

	if (kernel != m_kernels.end()) {
	   std::cout << "Found " << kernel->first <<'\n';
	   kernelOut = kernel->second;
   	} else {
	   std::cout << kernelName << " not found\n";
	}
}

cl::Context const &		CLEngine::getContext() const
{
	return m_context;
}
