
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

#include "Text2D.h"

// Constructor & Deconstructed
Text2D::Text2D() : transfer(NULL), texture(NULL), height(0), width(0)
{
}

Text2D::~Text2D()
{
}

void Text2D::setFont(FontAsset * asset, const char * text)
{
    // if the pointer is valid
    if (asset != NULL)
    {
        // set the text to be rendered
        asset->setText(text);

        if (texture == NULL) {
            texture = new GPU_Sampler(SINGLE_SAMPLER);
        }

        // and then generate the texture and send the data to the GPU
        SDL_Surface * surface = asset->generateTexture();
        texture->setBitmapData(surface->pixels,
            surface->w, surface->h,
            surface->format->BytesPerPixel,
            surface->format->Rmask
        );

        texture->send();

        // then grab the height and width of the object
        height = surface->h;
        width = surface->w;

        // Then free the surface generated
        SDL_FreeSurface(surface);
        data = std::string(text);
    }
}

std::string Text2D::getText()
{
    return data;
}

// This function sets the area the text will be rendered into
void Text2D::setArea(glm::vec2 size, ALIGNMENT align)
{
    // but before calling the other set area we will re arrange the position to match the alignment
    switch (align) 
    {
        case ALIGNMENT::CENTER:
        {
            glm::vec2 newSize(size);
            newSize[0] -= width / 2;
            newSize[1] -= height / 2;
            setArea(newSize);
            break;
        }
        
        case ALIGNMENT::LEFT:
        {
            glm::vec2 newSize(size);
            newSize[0] -= width;
            newSize[1] -= height / 2;
            setArea(newSize);
            break;
        }
    
        case ALIGNMENT::RIGHT:
        {
            setArea(size);
            break;
        }
    }
}

// sets the area which the text will be rendered into
void Text2D::setArea(glm::vec2 size)
{
    // create the list of vertices needed
    Vertices vert = {
        Vertex(size[0], size[1] + height, 0.1), Vertex(size[0] + width, size[1] + height, 0.1), Vertex(size[0] + width, size[1], 0.1),
        Vertex(size[0], size[1] + height, 0.1), Vertex(size[0], size[1], 0.1), Vertex(size[0] + width, size[1], 0.1),
    };

    // create the list of uvs needed
    Vertices uvs = {
        Vertex(0.0, 0.0, 0.0), Vertex(1.0, 0.0, 0.0), Vertex(1.0, 1.0, 0.0),
        Vertex(0.0, 0.0, 0.0), Vertex(0.0, 1.0, 0.0), Vertex(1.0, 1.0, 0.0),
    };

    if (transfer == NULL) {
        transfer = new GPU_Transfer();
    }

    transfer->setTextureCords(uvs);
    transfer->setVertices(vert);
    transfer->send();
}

// returns the ID for the texture
GpuID Text2D::getTextureID()
{
    return texture->getID();
}

// returns the ID for the vertex data
GpuID Text2D::getDataID()
{
    return transfer->getID();
}