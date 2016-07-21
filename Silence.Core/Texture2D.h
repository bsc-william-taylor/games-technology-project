
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
#include "GPU.h"

// This class stores all the data needed to draw a 2D texture
class SILENCE_EXPORT Texture2D
{
private:
    // The GPU transfer object for the vertices, uvs etc
    GPU_Transfer * transfer;
    // The sampler object which is the texture that should be bound
    GPU_Sampler * texture;
public:
    // Constructor & Deconstructor
    Texture2D();
    ~Texture2D();

    // Sets the texture that will be applied to the quad texture
    void setTexture(TextureAsset * assets);
    // Sets the area the texture will be rendered inside
    void setArea(glm::vec4 size);

    // returns the ID for the texture
    GPU_ID getTextureID();
    // returns the ID for the mesh
    GPU_ID getDataID();
};