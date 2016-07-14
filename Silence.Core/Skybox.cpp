
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

#include "Skybox.h"

// Constructor & Deconstructor
Skybox::Skybox()
	: cubemap_object_mesh(NULL),
	  cubemap_texture(NULL),
	  distance(-1)
{
}

Skybox::~Skybox()
{
	SAFE_RELEASE(cubemap_object_mesh);
	SAFE_RELEASE(cubemap_texture);
}

// creates the skybox by loading all the textures which represent each cube side
void Skybox::createSkybox(std::initializer_list<TextureAsset*> assets)
{
	// providing 6 textures our provided
	if (assets.size() == 6 && distance != -1)
	{
		// Create the cubemap sampler object
		cubemap_texture = new GPU_Sampler(CUBE_SAMPLER);

		// and pass each texture provided to it
		for (auto& texture : assets)
		{			
			if (texture != NULL)
			{
				cubemap_texture->setBitmapData(texture->getPixels(),
					texture->getWidth(),
					texture->getHeight(),
					texture->getBPP(),
					texture->getMask()
				);
			}
		}

		// and send the textures to the GPU
		cubemap_texture->send();

		// and then transfer the mesh data
		cubemap_object_mesh = new GPU_Transfer();
		cubemap_object_mesh->setVertices(vertices);
		cubemap_object_mesh->setNormals(normals);
		cubemap_object_mesh->send();
	}
}

// Sets the distance and calculates the meshes
void Skybox::setDistance(float distance)
{
	// The distance to render the cube at
	this->distance = distance;

	// calculate each vertex
	vertices = {
		// front
		Vertex(-distance, -distance, -distance), Vertex(distance, -distance, -distance), Vertex(distance, distance, -distance),
		Vertex(-distance, -distance, -distance), Vertex(-distance, distance, -distance), Vertex(distance, distance, -distance),
		// right
		Vertex(distance, -distance, -distance), Vertex(distance, distance, -distance), Vertex(distance, distance, distance),
		Vertex(distance, -distance, -distance), Vertex(distance, distance, distance), Vertex(distance, -distance, distance),
		// back
		Vertex(-distance, -distance, distance), Vertex(distance, -distance, distance), Vertex(distance, distance, distance),
		Vertex(-distance, -distance, distance), Vertex(-distance, distance, distance), Vertex(distance, distance, distance),
		// left
		Vertex(-distance, -distance, -distance), Vertex(-distance, distance, -distance), Vertex(-distance, distance, distance),
		Vertex(-distance, -distance, -distance), Vertex(-distance, distance, distance), Vertex(-distance, -distance, distance),
		// top
		Vertex(-distance, distance, -distance), Vertex(-distance, distance, distance), Vertex(distance, distance, distance),
		Vertex(-distance, distance, -distance), Vertex(distance, distance, distance), Vertex(distance, distance, -distance),
		// bottom
		Vertex(-distance, -distance, -distance), Vertex(-distance, -distance, distance), Vertex(distance, -distance, distance),
		Vertex(-distance, -distance, -distance), Vertex(distance, -distance, distance), Vertex(distance, -distance, -distance)
	};

	// then push back a null normal for each vertex
	for (auto& v : vertices) 
	{
		normals.push_back(Vertex(0.0, 0.0, 0.0));
	}
}

// returns the cubemaps texture ID
GPU_ID Skybox::getTextureID()
{
	return cubemap_texture->getID();
}

// returns the ID for the mesh
GPU_ID Skybox::getDataID()
{
	return cubemap_object_mesh->getID();
}