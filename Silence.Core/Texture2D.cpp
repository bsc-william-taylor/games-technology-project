
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

#include "Texture2D.h"

// Constructor & Deconstructor
Texture2D::Texture2D()
    : transfer(NULL),
      texture(NULL)
{
}

Texture2D::~Texture2D()
{
    SAFE_RELEASE(transfer);
    SAFE_RELEASE(texture);
}

// The following loads and send the texture data to the GPU ready for bounding to a mesh
void Texture2D::setTexture(TextureAsset * asset)
{
    // providing the asset pointer is valid
    if (asset != NULL)
    {
        if (texture == NULL)
        {
            texture = new GPU_Sampler(SINGLE_SAMPLER);
        } 

        // then pass the texture data ready for transfer
        texture->setBitmapData(asset->getPixels(),
            asset->getWidth(),
            asset->getHeight(),
            asset->getBPP(),
            asset->getMask()
        );

        // then send the texture to the GPU
        texture->send();
    }
}

// This sets the vertices data to the GPU and sets the texture ready for drawing
void Texture2D::setArea(glm::vec4 size)
{
    // First create the list of vertices needed
    Vertices vert = {
        Vertex(size[0], size[1] + size[3], 0.1), Vertex(size[0] + size[2], size[1] + size[3], 0.1), Vertex(size[0] + size[2], size[1], 0.1),
        Vertex(size[0], size[1] + size[3], 0.1), Vertex(size[0], size[1], 0.1), Vertex(size[0] + size[2], size[1], 0.1),
    };

    // Then the list of uv coords for bounding the texture
    Vertices uvs = {
        Vertex(0.0, 1.0, 0.0), Vertex(1.0, 1.0, 0.0), Vertex(1.0, 0.0, 0.0),
        Vertex(0.0, 1.0, 0.0), Vertex(0.0, 0.0, 0.0), Vertex(1.0, 0.0, 0.0),
    };

    if (transfer == NULL) {
        transfer = new GPU_Transfer();
    }

    // Then transfer the vertices and uvs to the GPU ready for drawing
    transfer->setTextureCords(uvs);
    transfer->setVertices(vert);
    transfer->send();
}

// returns the texture ID for the texture
GpuID Texture2D::getTextureID()
{
    return texture->getID();
}

// returns the object ID for the vertex data
GpuID Texture2D::getDataID()
{
    return transfer->getID();
}