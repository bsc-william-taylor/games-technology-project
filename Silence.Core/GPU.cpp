
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "GPU.h"

// Constructor & Deconstructor
GPU::GPU(GLboolean print)
	: grabbedGPU(false)
{
	// init glew to access 2.0+ gl funtions	
	if (glewInit() == GLEW_OK) 
	{
		// if successful and if pring it true print out details to the console
		shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		renderer = glGetString(GL_RENDERER);
		version = glGetString(GL_VERSION);
		vendor = glGetString(GL_VENDOR);

		// but only if the user specifies
		if (print) 
		{
			std::cout << "Shader Version : " << shaderVersion << std::endl;
			std::cout << "Renderer : " << renderer << std::endl;
			std::cout << "Version : " << version << std::endl;
			std::cout << "Vendor : " << vendor << std::endl;
		}

		grabbedGPU = true;
	}
	else 
	{
		// print an error message
		std::cout << ("Error grabbing GPU") << std::endl;
	}
}

GPU::~GPU()
{
}

// returns if a certain GPU API is supported
GLboolean GPU::has(GRAPHIC_API type, GLdouble version) 
{
	// if OPENGL is supported
	if (type == OPENGL)
	{
		// grab its version and check against the version requested
		GLfloat minVersion = (GLfloat)version;
		GLfloat f, s;
		
		glGetFloatv(GL_MAJOR_VERSION, &f);
		glGetFloatv(GL_MINOR_VERSION, &s);
		
		// return comparison
		return(f + s >= version);
	}
	else
	{
		// return false for now as no other API is supported.... yet
		return false;
	}
}