
#pragma once

#include "EngineLayer.h"
#include "TextureAsset.h"
#include "GPU.h"

class SILENCE_EXPORT Texture2D
{
    GPU_Transfer * transfer;
    GPU_Sampler * texture;
public:
    Texture2D();
    ~Texture2D();

    void setTexture(TextureAsset * assets);
    void setArea(glm::vec4 size);

    GpuID getTextureID();
    GpuID getDataID();
};