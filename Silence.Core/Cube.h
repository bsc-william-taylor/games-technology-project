
#pragma once

#include "TextureAsset.h"
#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT Cube
{
    GPU_Transfer * transfer;
    GPU_Sampler * texture;
public:
    Cube();
    ~Cube();

    void setArea(glm::vec3 position, glm::vec3 size, int texture_repeat_count);
    void setArea(glm::vec3 position, glm::vec3 size);
    void setTexture(TextureAsset * asset);

    GpuID getTextureID();
    GpuID getDataID();
};