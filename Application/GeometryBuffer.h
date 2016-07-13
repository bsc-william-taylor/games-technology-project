
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

// The list of render targets for my geometry buffer
enum GEOMETRY_BUFFER_ELEMENTS {
	GEOMETRY_BUFFER_TEXTURE_TYPE_POSITION,
	GEOMETRY_BUFFER_TEXTURE_TYPE_DIFFUSE,
	GEOMETRY_BUFFER_TEXTURE_TYPE_NORMAL,
};

// My geometry buffer class which helps setup and run deferred shading
class GeometryBuffer
{
private:
	// The ID for the final texture to be produced
	GLuint finalTexture;
	// the height of the GeometryBuffer
	GLuint bufferHeight;
	// the width of the GeometryBuffer
	GLuint bufferWidth;
	// the texture which stores depth and stencil values
	GLuint depthTexture;
	// and the ID for the combined frame buffer
	GLuint framebuffer;
public:
	// Constructor & Deconstructor
	GeometryBuffer();
	~GeometryBuffer();

	// This function returns the size of the geometry buffer (Width, Height)
	glm::vec2 getBufferSize();

	// This function sets up the geometry buffer by creating the multiple render targets
	GLvoid setupGeometryBuffer(int w, int h);
	// This function prepares the buffer to be read from by shaders
	GLvoid prepareForReading();
	// This function prepares the buffer to be written to by shaders
	GLvoid prepareForWriting();
	// This is a function that does some extra things to prepare for the start of the frame
	GLvoid startFrame();
	// This function does the final copy of the image produced in FinalTexture to the frameb uffer
	GLvoid stopFrame();
private:
	// The ID of the texture which stores positions 
	GLuint positionTexture;
	// The ID of the texture which stores the diffuse colour value
	GLuint diffuseTexture;
	// The ID of the texture which stores the normals for each object
	GLuint normalTexture;
};