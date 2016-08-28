
#pragma once

#include "EngineLayer.h"
#include "FontAsset.h"
#include "GPU.h"

enum class Alignment { Center, Right, Left };

class SILENCE_EXPORT Text2D
{
    std::string data;

    GPU_Transfer * transfer;
    GPU_Sampler * texture;

    GLuint height;
    GLuint width;
public:
    Text2D();
    ~Text2D();

    void setFont(FontAsset * text_font, const char * text);
    void setArea(glm::vec2 size, Alignment textAlignment);
    void setArea(glm::vec2 size);

    std::string getText();

    GpuID getTextureID();
    GpuID getDataID();
};
