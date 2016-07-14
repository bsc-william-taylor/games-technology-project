
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

#pragma once

#include "EngineLayer.h"
#include "GPU.h"

// The following is the output shader for the deferred renderer where lighting takes place
class SILENCE_EXPORT DeferredOutputShader
{
private:
	// The vector of point lights to renderer
	//std::vector<PointLight> lights;
	// The LLGI program object which simplifies creating shader
	GPU_Program * program;
	// our uniform buffer object which stores all static lights
	GLuint UBO;
public:
	// Constructor & Deconstructor
	DeferredOutputShader();
	~DeferredOutputShader();

	// returns a pointer to the LLGI program object
	GPU_Program * getProgram();

	//
	void runIndex(int ID, int numberOfIndices);
	// binds a VAO / object ID
	void prepare(int ID);
	// This functions compiles the shader source so its ready for use
	void compile();
	// render the object vertices from v_start, v_end
	void run(int v_start, int v_end);
	// render this number of vertices
	void run(int vertex_num);
};