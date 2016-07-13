
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

#ifndef __SKYBOX__H_
#define __SKYBOX__H_

#include "TextureAsset.h"
#include "GPU.h"

// The side of the skybox
enum SKYBOX_SIDE { 
	RIGHT, FRONT, BACK, LEFT, TOP
};

// A class which stores all data needed for drawing a cube
class Skybox 
{
private:
	// This transfer object sends the mesh data
	GPU_Transfer * cubemap_object_mesh;
	// This transfer object sends the texture data
	GPU_Sampler * cubemap_texture;
public:
	// Constructor & Deconstructor
	Skybox();
	~Skybox();

	// Creates the skybox where assets is the list of textures for the skybox
	void createSkybox(std::initializer_list<TextureAsset*> assets);
	// Sets the distance to render the skybox
	void setDistance(float distance);

	// returns the ID for cubemap texture
	GPU_ID getTextureID();
	// returns the ID for mesh data
	GPU_ID getDataID();
private:
	// The vertices for the cube
	Vertices vertices;
	// The normals for the cube
	Vertices normals;
	// The uvs for the cube
	Vertices uvs;

	// the distance to render it at
	GLfloat distance;
};

#endif