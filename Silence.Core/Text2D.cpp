
#include "Text2D.h"

Text2D::Text2D() : 
    transfer(nullptr),
    texture(nullptr), 
    height(0), 
    width(0)
{
}

Text2D::~Text2D()
{
    SAFE_RELEASE(transfer);
    SAFE_RELEASE(texture);
}

void Text2D::setFont(FontAsset * asset, const char * text)
{
    if (asset != nullptr)
    {
        asset->setText(text);
        
        const auto surface = asset->generateTexture();
        texture = texture ? texture : new GPU_Sampler(SINGLE_SAMPLER);
        texture->setBitmapData(surface->pixels,
            surface->w, surface->h,
            surface->format->BytesPerPixel,
            surface->format->Rmask
        );

        texture->send();

        height = surface->h;
        width = surface->w;

        SDL_FreeSurface(surface);
        data = std::string(text);
    }
}

std::string Text2D::getText()
{
    return data;
}

void Text2D::setArea(glm::vec2 size, Alignment align)
{
    switch (align) 
    {
        case Alignment::Center:
        {
            glm::vec2 newSize(size);
            newSize[0] -= width / 2;
            newSize[1] -= height / 2;
            setArea(newSize);
            break;
        }
        
        case Alignment::Left:
        {
            glm::vec2 newSize(size);
            newSize[0] -= width;
            newSize[1] -= height / 2;
            setArea(newSize);
            break;
        }
    
        case Alignment::Right:
        {
            setArea(size);
            break;
        }
    }
}

void Text2D::setArea(glm::vec2 size)
{
    Vertices vert = {
        Vertex(size[0], size[1] + height, 0.1), 
        Vertex(size[0] + width, size[1] + height, 0.1), 
        Vertex(size[0] + width, size[1], 0.1),
        Vertex(size[0], size[1] + height, 0.1), 
        Vertex(size[0], size[1], 0.1), 
        Vertex(size[0] + width, size[1], 0.1),
    };

    Vertices uvs = {
        Vertex(0.0, 0.0, 0.0), Vertex(1.0, 0.0, 0.0), Vertex(1.0, 1.0, 0.0),
        Vertex(0.0, 0.0, 0.0), Vertex(0.0, 1.0, 0.0), Vertex(1.0, 1.0, 0.0),
    };

    transfer = transfer ? transfer : new GPU_Transfer();
    transfer->setTextureCords(uvs);
    transfer->setVertices(vert);
    transfer->send();
}

GpuID Text2D::getTextureID()
{
    return texture->getID();
}

GpuID Text2D::getDataID()
{
    return transfer->getID();
}