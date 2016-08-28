#include "Model.h"

Model::Model()
    : mesh(nullptr), file(nullptr)
{
    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);
}

Model::~Model()
{
    for(auto& sampler : samplers)
    {
        SAFE_RELEASE(sampler);
    }

    SAFE_RELEASE(mesh);
}

ModelAsset * Model::getModelAsset()
{
    return file;
}

GLuint Model::getMaterialID(GLuint i)
{
    return file->getMeshes()[i]->MaterialIndex;
}

void Model::setModel(ModelAsset * assets)
{
    if (assets != nullptr && mesh == nullptr)
    {
        auto meshes = assets->getMeshes();
        
        mesh = new GPU_Transfer();
        mesh->setTextureCords(assets->getUvs());
        mesh->setVertices(assets->getVertices());
        mesh->setNormals(assets->getNormals());
        mesh->setIndices(assets->getIndices());
        mesh->send();

        auto textures = assets->getTextures();

        for (auto i = 0; i < textures.size(); i++) 
        {
            const auto texture_asset = textures[i];

            if (texture_asset != nullptr)
            {
                const auto texture = new GPU_Sampler(SINGLE_SAMPLER);
                texture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
                texture->setBitmapWrapping(GL_REPEAT, GL_REPEAT);
                texture->setBitmapData(texture_asset->getPixels(),
                    texture_asset->getWidth(),
                    texture_asset->getHeight(),
                    texture_asset->getBPP(),
                    texture_asset->getMask()
                );

                texture->send();

                samplers.push_back(texture);
            }
            else
            {
                samplers.push_back(nullptr);
            }
        }

        file = assets;	
    }
}

SolidCylinder * Model::createCylinder(glm::mat4 mat, double)
{
    auto solid = new SolidCylinder();
    auto meshs = file->getMeshes();

    for (auto i = 0; i < meshs.size(); i++)
    {
        for (auto& vb : file->getVertices())
        {
            auto v = glm::vec3(mat * glm::vec4(vb, 1.0));

            if (v.x < min.x)	min.x = v.x;
            if (v.y < min.y)	min.y = v.y;
            if (v.z < min.z)	min.z = v.z;
            if (v.x > max.x)	max.x = v.x;
            if (v.y > max.y)	max.y = v.y;
            if (v.z > max.z)	max.z = v.z;
        }
    }

    auto center = (max - min) / glm::vec3(2, 2, 2);
    
    double radius = glm::length(max - center);
    
    solid->setPositionVector(center);
    solid->setRadius(radius);

    return solid;
}

SolidBox * Model::createbox(glm::mat4 mat)
{
    auto solid = new SolidBox();
    auto meshs = file->getMeshes();

    for (auto i = 0; i < meshs.size(); i++)
    {
        for (auto& vb : file->getVertices())
        {
            auto v = glm::vec3(mat * glm::vec4(vb, 1.0));

            if (v.x < min.x)	min.x = v.x;
            if (v.y < min.y)	min.y = v.y;
            if (v.z < min.z)	min.z = v.z;
            if (v.x > max.x)	max.x = v.x;
            if (v.y > max.y)	max.y = v.y;
            if (v.z > max.z)	max.z = v.z;
        }
    }

    
    solid->setPositionVector(glm::vec3(min));
    solid->setSizeVector(glm::vec3(max));

    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);

    return solid;
}

ModelAsset * Model::getMeshes() 
{
    return file;
}

GPU_Transfer * Model::getMesh() 
{
    return mesh;
}

vector<GPU_Sampler *>& Model::getTextures() 
{
    return samplers;
}