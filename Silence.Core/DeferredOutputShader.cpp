
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

#include "DeferredOutputShader.h"

// Constructor & Deconstructor
DeferredOutputShader::DeferredOutputShader()
	: program(NULL)
{
}

DeferredOutputShader::~DeferredOutputShader()
{
	SAFE_RELEASE(program)
}

void DeferredOutputShader::compile()
{
	// Make sure we only compile once
	if (program == NULL)
	{
		// Create the GPU program object
		program = new GPU_Program();
		// Set the vertex source for the program
		program->setVertexSource(SRC(
			layout(location = 0) in vec3 positions;
			layout(location = 1) in vec3 normals;
			layout(location = 2) in vec3 uvs;

			uniform mat4 projectionMatrix;
			uniform mat4 model;
			uniform mat4 view;

			void main() {
				gl_Position = projectionMatrix * view * model * vec4(positions, 1.0);
			}
		));

		// Set the fragment source for the program
		program->setFragmentSource(SRC(
			layout(location = 0) out vec4 fragment_colour;

			const int NULL_PASS = - 1;
			const int POINT_PASS = 1;
			const int SUN_PASS = 0;

			struct PointLight {
				vec4 pointPosition;
				vec4 pointColour; 
				vec4 att; // constantA, ambientI, linearA, expA;			
			};

			struct DirectionalLight {
				vec4 lightDirection;
				vec4 lightColour;
			};

			layout(std140) uniform SceneLights {
				DirectionalLight sceneLight;
				PointLight pointLights[50];
			};
			
			uniform sampler2D PositionTexture;
			uniform sampler2D NormalTexture;
			uniform sampler2D ColourTexture;
			uniform vec2 screenSize;
			uniform int lightPass;
			uniform int pointID;
		
			vec4 materialPosition;
			vec4 materialColour;
			vec4 materialNormal;

			vec4 getPointLight()
			{
				vec3 direction = materialPosition.xyz - pointLights[pointID].pointPosition.xyz;
	
				float diffuseAngle = max(0.0, dot(materialNormal.xyz, -normalize(direction)));

				float distanceToLight = length(direction);
				float fAttTotal = pointLights[pointID].att.x + pointLights[pointID].att.z *distanceToLight + pointLights[pointID].att.w*distanceToLight*distanceToLight;

				return vec4(pointLights[pointID].pointColour * (pointLights[pointID].att.y + diffuseAngle) / fAttTotal);
			}

			vec4 getSunLight()
			{
				float sunLightI = max(0.0, dot(materialNormal.xyz, -normalize(sceneLight.lightDirection.xyz)));

				return vec4((sceneLight.lightColour * (sunLightI + sceneLight.lightColour.w)));
			}
	
			void main()
			{
				if (lightPass != NULL_PASS)
				{
					vec2 uv_s = gl_FragCoord.xy / screenSize;

					materialPosition = texture(PositionTexture, uv_s);
					materialColour = texture(ColourTexture, uv_s);
					materialNormal = texture(NormalTexture, uv_s);

					if (materialNormal.xyz != vec3(0.0, 0.0, 0.0))
					{
						if (lightPass == SUN_PASS)
						{
							fragment_colour = (materialColour *  getSunLight());
						}

						if (lightPass == POINT_PASS)
						{
							fragment_colour = (materialColour * getPointLight());
						}
					}
					else
					{
						fragment_colour = materialColour;
					}
				}
				else
				{
					fragment_colour = vec4(0.5, 0.5, 0.5, 1.0);
				}
				
			}
		));

		// then compile the program
		if (!program->compile())
		{
			throw Error(SYSTEM, "couldnt compile deferred output shader");
		}
	}
}

void DeferredOutputShader::runIndex(int ID, int indicesSize)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
 
// runs the program drawing vertexCount number of vertices
void DeferredOutputShader::run(int vertexCount)
{
	program->run(vertexCount);
}

// runs the program drawing the vertices through i to x
void DeferredOutputShader::run(int i, int x)
{
	program->run(i, x);
}

// prepares the program for execution
void DeferredOutputShader::prepare(int ID)
{
	// prepares the object by binding an ID 
	program->prepare(ID);

	// get the programs ID
	GLuint programID = program->getID();

	// bind the multiple textures needed for 
	GLuint positionT = glGetUniformLocation(programID, "PositionTexture");
	GLuint normalT = glGetUniformLocation(programID, "NormalTexture");
	GLuint colourT = glGetUniformLocation(programID, "ColourTexture");
	
	// then bind the texture IDs
	glUniform1i(positionT, 0);
	glUniform1i(normalT, 1);
	glUniform1i(colourT, 2);
}

// returns a pointer to the LLGI program object
GPU_Program * DeferredOutputShader::getProgram()
{
	return program;
}