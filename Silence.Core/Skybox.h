

#pragma once

#include "TextureAsset.h"
#include "GPU.h"

enum SkyboxSide { 
    Right, Front, Back, Left, Top
};

class SILENCE_EXPORT Skybox
{
    GPU_Transfer * cubemap_object_mesh;
    GPU_Sampler * cubemap_texture;

    Vertices vertices;
    Vertices normals;
    Vertices uvs;

    GLfloat distance;
public:
    Skybox();
    ~Skybox();

    void createSkybox(std::initializer_list<TextureAsset*> assets);
    void setDistance(float distance);

    GpuID getTextureID();
    GpuID getDataID();
};
