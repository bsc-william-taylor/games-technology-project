
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
#include "TextureAsset.h"
#include "math.h"

using std::vector;
using glm::vec3;
using glm::vec2;

class SILENCE_EXPORT ModelAsset
{
private:
	struct MeshEntry {
		unsigned int MaterialIndex;
		unsigned int NumIndices;
		unsigned int VertexStart;
		void * IndexStart;
	};

	vector<unsigned int> indices;
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec3> uvs;

	// the vector of all the textures the model uses
	vector<TextureAsset *> textures;
	vector<MeshEntry *> meshes;
	// the material ID vector for each mesh

public:
	// Constructor & Deconstructor
	ModelAsset(std::string name);
	~ModelAsset();

	vector<unsigned int>& getIndices();
	vector<vec3>& getVertices();
	vector<vec3>& getNormals();
	vector<vec3>& getUvs();

	vector<TextureAsset *>& getTextures();
	vector<MeshEntry *>& getMeshes();

	// loads the model from the given filename
	void grabFromFile(const char * filename);
	void recycle();
	// returns the name of the asset
	std::string getName();

private:
	// a load of private function to help with the setup of meshes, materials and the scene
	bool setupMaterials(const aiScene * scene, const std::string& filename);
	bool setupFromScene(const aiScene * scene, const std::string& filename);
	bool setupMesh(unsigned int ID, const aiMesh * mesh);
private:
	// the name of the asset
	std::string name;
};