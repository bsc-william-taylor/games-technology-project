
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

#include "DeferredInputShader.h"

// Constructor & Deconstructor
DeferredInputShader::DeferredInputShader()
	: program(NULL)
{
}

DeferredInputShader::~DeferredInputShader()
{
	SAFE_RELEASE(program)
}

void DeferredInputShader::compile()
{
	// make sure the program only compiles once
	if (!program)
	{
		// create the LLGI program object
		program = new GPU_Program();
		// then set the fragment source for the shader
		program->setFragmentSource(SRC(
			layout(location = 0) out vec3 PositionData;
			layout(location = 1) out vec3 NormalData;
			layout(location = 2) out vec3 ColorData;

			uniform sampler2D sampler;
			uniform samplerCube cubeMap;
			uniform int sampler_index;

			in vec3 position;
			in vec3 normal;
			in vec3 uv_cords;

			void main() 
			{
				if (sampler_index == 0)
				{
					ColorData = texture(sampler, uv_cords.xy).rgb;
				}
				else
				{
					ColorData = texture(cubeMap, uv_cords).rgb;
				}

				PositionData = position;
				NormalData = normal;
			}
		));

		// set the vertex source for the shader
		program->setVertexSource(SRC(
			layout(location = 0) in vec3 positions;
			layout(location = 1) in vec3 normals;
			layout(location = 2) in vec3 uvs;

			uniform mat4 projection;
			uniform mat4 model;
			uniform mat4 view;
			uniform int sampler_index;

			out vec3 position;
			out vec3 normal;
			out vec3 uv_cords;

			void main() {
				gl_Position = projection * view * model * vec4(positions, 1.0);

				position = (model * vec4(positions, 1.0)).xyz;
				normal = normalize((vec4(normals, 1.0))).xyz;

				if (sampler_index == 0)
				{
					uv_cords = uvs;
				}
				else
				{
					uv_cords = positions;
				}
				
			}
		));

		// compiles the shader source and throws exception is it fails
		if (!program->compile())
		{
			throw Error(SYSTEM, "couldnt compile deferred input shader");
		}
	}
}

// runs the program and draws the vertexCount number of vertices
void DeferredInputShader::run(int vertexCount)
{
	program->run(vertexCount);
}

// prepares the object by binding the VAO
void DeferredInputShader::prepare(int ID)
{
	program->prepare(ID);
}

void DeferredInputShader::runIndex(int ID, int indicesSize)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

// runs the program with these specified geometry type and draws all vertices through i to x
void DeferredInputShader::run(int a, int i, int x)
{
	program->run(a, i, x);
}

// runs the program drawing the vertices though i to x
void DeferredInputShader::run(int i, int x)
{
	program->run(GL_TRIANGLES, i, x);
}

// binds the vao and binds a texture as well
void DeferredInputShader::prepare(int ID, int tID)
{
	program->prepare(ID);
	program->bind(tID);
}

// returns a pointer to the GPU_Program
GPU_Program * DeferredInputShader::getProgram()
{
	return program;
}