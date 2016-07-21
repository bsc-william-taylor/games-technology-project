
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

#include "Model.h"

// Constructor & Deconstructor
Model::Model()
    : mesh(NULL), file(NULL)
{
    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);
}

Model::~Model()
{
    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);
}

// returns a direct pointer to the model file used to generate the model
ModelAsset * Model::getModelAsset()
{
    return file;
}

GLuint Model::getMaterialID(GLuint i)
{
    return file->getMeshes()[i]->MaterialIndex;
}

void Model::setModel(ModelAsset * assets)
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

SolidCylinder * Model::createCylinder(glm::mat4 mat, double)
{
    SolidCylinder * solid = new SolidCylinder();

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

    glm::vec3 center = (max - min) / glm::vec3(2, 2, 2);
    
    double radius = glm::length(max - center);
    
    solid->setPositionVector(center);
    solid->setRadius(radius);

    return solid;
}

SolidBox * Model::createbox(glm::mat4 mat)
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