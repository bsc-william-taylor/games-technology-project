
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

#include "Cube.h"

// Constructor & Deconstructor
Cube::Cube()
    : transfer(NULL),
      texture(NULL)
{
}

Cube::~Cube()
{
    SAFE_RELEASE(transfer);
    SAFE_RELEASE(texture);
}

// The following sets the texture for the cubes sides
void Cube::setTexture(TextureAsset * asset)
{
    // if the pointer is valid
    if (asset != NULL)
    {
        // load the texture data from the asset
        texture = new GPU_Sampler(SINGLE_SAMPLER);
        texture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        texture->setBitmapData(asset->getPixels(),
            asset->getWidth(),
            asset->getHeight(),
            asset->getBPP(),
            asset->getMask()
        );

        // and send the data to the GPU
        texture->send();
    }
}

// This function sets the area the cube should be rendered into
void Cube::setArea(glm::vec3 position, glm::vec3 size)
{
    // Just call the other method setting repeat to 1
    setArea(position, size, 1);
}

// This function sets the area of the cube in 3D space
void Cube::setArea(glm::vec3 position, glm::vec3 size, int r)
{
    // make sure we only load once
    if(transfer == NULL)
    {
        // first calculate the dimensions for the object
        glm::vec3 s = position + size;
        glm::vec3 p = position;

        // The create the list of vertices for the cube
        Vertices vert = {
            glm::vec3(p[0], p[1], p[2]), glm::vec3(p[0], s[1], p[2]), glm::vec3(s[0], s[1], p[2]),
            glm::vec3(p[0], p[1], p[2]), glm::vec3(s[0], p[1], p[2]), glm::vec3(s[0], s[1], p[2]),

            glm::vec3(p[0], p[1], s[2]), glm::vec3(p[0], s[1], s[2]), glm::vec3(s[0], s[1], s[2]),
            glm::vec3(p[0], p[1], s[2]), glm::vec3(s[0], p[1], s[2]), glm::vec3(s[0], s[1], s[2]),

            glm::vec3(s[0], p[1], p[2]), glm::vec3(s[0], s[1], p[2]), glm::vec3(s[0], s[1], s[2]),
            glm::vec3(s[0], p[1], p[2]), glm::vec3(s[0], p[1], s[2]), glm::vec3(s[0], s[1], s[2]),

            glm::vec3(p[0], p[1], p[2]), glm::vec3(p[0], s[1], p[2]), glm::vec3(p[0], s[1], s[2]),
            glm::vec3(p[0], p[1], p[2]), glm::vec3(p[0], p[1], s[2]), glm::vec3(p[0], s[1], s[2]),

            glm::vec3(p[0], s[1], p[2]), glm::vec3(p[0], s[1], s[2]), glm::vec3(s[0], s[1], s[2]),
            glm::vec3(p[0], s[1], p[2]), glm::vec3(s[0], s[1], p[2]), glm::vec3(s[0], s[1], s[2]),

            glm::vec3(p[0], p[1], p[2]), glm::vec3(p[0], p[1], s[2]), glm::vec3(s[0], p[1], s[2]),
            glm::vec3(p[0], p[1], p[2]), glm::vec3(s[0], p[1], p[2]), glm::vec3(s[0], p[1], s[2])
        };

        // The create the list of normals for the cube
        Vertices normals = {
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0)
        };
        
        // The create the list of uvs for the cube
        Vertices uv_cords = {
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(0.0*r, 1.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(0.0*r, 1.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(0.0*r, 1.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(0.0*r, 1.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(0.0*r, 1.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(0.0*r, 1.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
            glm::vec3(0.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 0.0*r, 0.0), glm::vec3(1.0*r, 1.0*r, 0.0),
        };

        // Create the GPU transfer object which will send the data to the GPU
        transfer = new GPU_Transfer();
        transfer->setTextureCords(uv_cords);
        transfer->setNormals(normals);
        transfer->setVertices(vert);
        transfer->send();
    }
}

// returns the ID for the cubes texture
GPU_ID Cube::getTextureID()
{
    return transfer->getID();
}

// returns the ID for the cubes vertices, normals etc
GPU_ID Cube::getDataID()
{
    return texture->getID();
}