
#include "Skybox.h"

Skybox::Skybox()
    : cubemap_object_mesh(nullptr),
      cubemap_texture(nullptr),
      distance(-1)
{
}

Skybox::~Skybox()
{
    SAFE_RELEASE(cubemap_object_mesh);
    SAFE_RELEASE(cubemap_texture);
}

void Skybox::createSkybox(std::initializer_list<TextureAsset*> assets)
{
    if (assets.size() == 6 && distance != -1 && !cubemap_texture)
    {
        cubemap_texture = new GPU_Sampler(CUBE_SAMPLER);

        for (auto& texture : assets)
        {			
            if (texture != nullptr)
            {
                cubemap_texture->setBitmapData(texture->getPixels(),
                    texture->getWidth(),
                    texture->getHeight(),
                    texture->getBPP(),
                    texture->getMask()
                );
            }
        }

        cubemap_texture->send();

        if(!cubemap_texture)
        {
            cubemap_object_mesh = new GPU_Transfer();
            cubemap_object_mesh->setVertices(vertices);
            cubemap_object_mesh->setNormals(normals);
            cubemap_object_mesh->send();
        }
    }
}

void Skybox::setDistance(float distance)
{
    this->distance = distance;

    vertices = {
        // front
        Vertex(-distance, -distance, -distance), Vertex(distance, -distance, -distance), Vertex(distance, distance, -distance),
        Vertex(-distance, -distance, -distance), Vertex(-distance, distance, -distance), Vertex(distance, distance, -distance),
        // right
        Vertex(distance, -distance, -distance), Vertex(distance, distance, -distance), Vertex(distance, distance, distance),
        Vertex(distance, -distance, -distance), Vertex(distance, distance, distance), Vertex(distance, -distance, distance),
        // back
        Vertex(-distance, -distance, distance), Vertex(distance, -distance, distance), Vertex(distance, distance, distance),
        Vertex(-distance, -distance, distance), Vertex(-distance, distance, distance), Vertex(distance, distance, distance),
        // left
        Vertex(-distance, -distance, -distance), Vertex(-distance, distance, -distance), Vertex(-distance, distance, distance),
        Vertex(-distance, -distance, -distance), Vertex(-distance, distance, distance), Vertex(-distance, -distance, distance),
        // top
        Vertex(-distance, distance, -distance), Vertex(-distance, distance, distance), Vertex(distance, distance, distance),
        Vertex(-distance, distance, -distance), Vertex(distance, distance, distance), Vertex(distance, distance, -distance),
        // bottom
        Vertex(-distance, -distance, -distance), Vertex(-distance, -distance, distance), Vertex(distance, -distance, distance),
        Vertex(-distance, -distance, -distance), Vertex(distance, -distance, distance), Vertex(distance, -distance, -distance)
    };

    for (auto& v : vertices) 
    {
        normals.push_back(Vertex(0.0, 0.0, 0.0));
    }
}

GpuID Skybox::getTextureID()
{
    return cubemap_texture->getID();
}

GpuID Skybox::getDataID()
{
    return cubemap_object_mesh->getID();
}