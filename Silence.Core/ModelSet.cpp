

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

#include "ModelSet.h"

// Constructor & Deconstructor
ModelSet::ModelSet()
	: mesh(NULL), file(NULL)
{
	min = Vertex(1000000, 1000000, 1000000);
	max = Vertex(-1000000, -1000000, -1000000);
}

ModelSet::~ModelSet()
{
	min = Vertex(1000000, 1000000, 1000000);
	max = Vertex(-1000000, -1000000, -1000000);
}

// returns a direct pointer to the model file used to generate the model
ModelAsset * ModelSet::getModelAsset()
{
	return file;
}

GLuint ModelSet::getMaterialID(GLuint i)
{
	return file->getMeshes()[i]->MaterialIndex;
}

GLuint ModelSet::getIndices()
{
	return indices;
}

void ModelSet::setModel(ModelAsset * assets)
{
	// make sure the pointer is valid
	if (assets != NULL && mesh == NULL)
	{
		auto meshes = assets->getMeshes();

		mesh = new GPU_Transfer();
		mesh->setTextureCords(assets->getUvs());
		mesh->setVertices(assets->getVertices());
		mesh->setNormals(assets->getNormals());
		mesh->setIndices(assets->getIndices());
		mesh->send();

		indices = assets->getIndices().size();

		// then get the list of textures the model uses
		auto textures = assets->getTextures();

		// then loop through and transfer each textures data to the gpu
		for (int i = 0; i < textures.size(); i++)
		{
			TextureAsset * texture_asset = textures[i];

			if (texture_asset != NULL)
			{
				GPU_Sampler * texture = new GPU_Sampler(SINGLE_SAMPLER);

				texture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
				texture->setBitmapWrapping(GL_REPEAT, GL_REPEAT);
				texture->setBitmapData(texture_asset->getPixels(),
					texture_asset->getWidth(),
					texture_asset->getHeight(),
					texture_asset->getBPP(),
					texture_asset->getMask()
				
				);

				texture->send();

				// then store the IDs in our vector so we can use them when drawing
				tIDs.push_back(texture->getID());
			}
			else
			{
				tIDs.push_back(NULL);
			}
		}

		file = assets;
	}
}

SolidBox * ModelSet::createbox(glm::mat4 mat)
{
	SolidBox * solid = new SolidBox();

	auto meshs = file->getMeshes();

	for (int i = 0; i < meshs.size(); i++)
	{
		for (auto& vb : file->getVertices())
		{
			glm::vec3 v = glm::vec3(mat * glm::vec4(vb, 1.0));

			if (v.x < min.x)	min.x = v.x;
			if (v.y < min.y)	min.y = v.y;
			if (v.z < min.z)	min.z = v.z;
			if (v.x > max.x)	max.x = v.x;
			if (v.y > max.y)	max.y = v.y;
			if (v.z > max.z)	max.z = v.z;
		}
	}


	solid->setPositionVector(glm::vec3(min));
	solid->setSizeVector(glm::vec3(max));

	min = Vertex(1000000, 1000000, 1000000);
	max = Vertex(-1000000, -1000000, -1000000);

	return solid;
}

SolidCylinder * ModelSet::createCylinder(glm::mat4 mat, double div)
{
	SolidCylinder * solid = new SolidCylinder();

	auto meshs = file->getMeshes();

	for (int i = 0; i < meshs.size(); i++)
	{
		for (auto& vb : file->getVertices())
		{
			glm::vec3 v = glm::vec3(mat * glm::vec4(vb, 1.0));

			if (v.x < min.x)	min.x = v.x;
			if (v.y < min.y)	min.y = 0.0;
			if (v.z < min.z)	min.z = v.z;
			if (v.x > max.x)	max.x = v.x;
			if (v.y > max.y)	max.y = 0.0;
			if (v.z > max.z)	max.z = v.z;
			
		}
	}

	glm::vec3 center = (max + min) / glm::vec3(2.0, 2.0, 2.0);

	double radius = glm::distance(center, max);

	solid->setPositionVector(center);
	solid->setRadius(radius / div);

	min = Vertex(1000000, 1000000, 1000000);
	max = Vertex(-1000000, -1000000, -1000000);

	return solid;
}

GLuint ModelSet::getSetSize()
{
	return setSize;
}

GLvoid ModelSet::bindModelMatrixArray()
{
	glBindBuffer(GL_ARRAY_BUFFER, matricesID);
}

void ModelSet::setMatrices(std::vector<glm::mat4> matrices)
{
	glBindVertexArray(mesh->getID());
	glGenBuffers(1, &matricesID);
	glBindBuffer(GL_ARRAY_BUFFER, matricesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * matrices.size(), matrices.data(), GL_STATIC_DRAW);
	setSize = matrices.size();

	for (unsigned int z = 0; z < 4; z++) {
		glEnableVertexAttribArray(4 + z);
		glVertexAttribPointer(4 + z, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * z * 4));
		glVertexAttribDivisor(4 + z, 1);
	}
}