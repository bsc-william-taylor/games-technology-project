
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

#ifndef __CUBE__H_
#define __CUBE__H_

#include "TextureAsset.h"
#include "EngineLayer.h"
#include "GPU.h"

// This function sets up the data needed to render a cube
class SILENCE_EXPORT Cube
{
private:
	// The transfer object which sends the vertices needed
	GPU_Transfer * transfer;
	// The sampler object which sends the bitmap data needed
	GPU_Sampler * texture;
public:
	// Constructor & Deconstructor
	Cube();
	~Cube();

	// sets the area of the cube and the amount of times you want the texture to repeat on the surface
	void setArea(glm::vec3 position, glm::vec3 size, int texture_repeat_count);
	// sets the area of the cube
	void setArea(glm::vec3 position, glm::vec3 size);
	// sets the texture which will be bound to each face
	void setTexture(TextureAsset * asset);

	// returns the ID for the texture
	GPU_ID getTextureID();
	// returns the ID for the mesh
	GPU_ID getDataID();
};

#endif