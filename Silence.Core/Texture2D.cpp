
#include "Texture2D.h"

Texture2D::Texture2D()
    : transfer(nullptr),
      texture(nullptr)
{
}

Texture2D::~Texture2D()
{
    SAFE_RELEASE(transfer);
    SAFE_RELEASE(texture);
}

void Texture2D::setTexture(TextureAsset * asset)
{
    if (asset != nullptr)
    {
        if (texture == nullptr)
        {
            texture = new GPU_Sampler(SINGLE_SAMPLER);
        } 

        texture->setBitmapData(asset->getPixels(),
            asset->getWidth(),
            asset->getHeight(),
            asset->getBPP(),
            asset->getMask()
        );

        texture->send();
    }
}

void Texture2D::setArea(glm::vec4 size)
{
    Vertices vert = {
        Vertex(size[0], size[1] + size[3], 0.1), 
        Vertex(size[0] + size[2], size[1] + size[3], 0.1), 
        Vertex(size[0] + size[2], size[1], 0.1),
        Vertex(size[0], size[1] + size[3], 0.1), 
        Vertex(size[0], size[1], 0.1), 
        Vertex(size[0] + size[2], size[1], 0.1),
    };

    Vertices uvs = {
        Vertex(0.0, 1.0, 0.0), Vertex(1.0, 1.0, 0.0), Vertex(1.0, 0.0, 0.0),
        Vertex(0.0, 1.0, 0.0), Vertex(0.0, 0.0, 0.0), Vertex(1.0, 0.0, 0.0),
    };

    transfer = transfer ? transfer : new GPU_Transfer();
    transfer->setTextureCords(uvs);
    transfer->setVertices(vert);
    transfer->send();
}

GpuID Texture2D::getTextureID()
{
    return texture->getID();
}

GpuID Texture2D::getDataID()
{
    return transfer->getID();
}