
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

#ifndef __MODEL__H_
#define __MODEL__H_

#include "EngineLayer.h"
#include "ModelFile.h"
#include "World.h"
#include "math.h"
#include "GPU.h"

// This class loads all the data needed to render a model on the screen
class Model
{
private:
	//
	std::vector<GPU_ID> tIDs;
	// a vector of IDs for the textures
	GPU_Transfer * mesh;
	// The model asset file
	ModelAsset * file;
	//
	Vertex min, max;
public:
	// Constructor & Deconstructor
	Model();
	~Model();

	// sets the asset which will contain vertex etc information
	GLvoid setModel(ModelAsset * model);
	
	SolidCylinder * createCylinder(glm::mat4, double);
	SolidBox * createbox(glm::mat4);
	//
	ModelAsset * getMeshes() {
		return file;
	}

	GPU_Transfer * getMesh() {
		return mesh;
	}

	std::vector<GPU_ID>& getTextures() {
		return tIDs;
	}

	GLuint getMaterialID(GLuint);


	// returns a pointer to the model asset
	ModelAsset * getModelAsset();
};

#endif