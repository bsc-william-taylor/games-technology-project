
#include "ModelSet.h"

ModelSet::ModelSet() : 
    file(nullptr),
    mesh(nullptr)
{
    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);
}

ModelSet::~ModelSet()
{
    SAFE_RELEASE(mesh);

    for(auto& sampler : samplers)
    {
        SAFE_RELEASE(sampler);
    }
}

ModelAsset * ModelSet::getModelAsset()
{
    return file;
}

GLuint ModelSet::getMaterialID(GLuint i)
{
    return file->getMeshes()[i]->MaterialIndex;
}

GLuint ModelSet::getIndices()
{
    return indices;
}

void ModelSet::setModel(ModelAsset * assets)
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

        indices = assets->getIndices().size();

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

SolidBox * ModelSet::createbox(glm::mat4 mat)
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

    solid->setPositionVector(vec3(min));
    solid->setSizeVector(vec3(max));

    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);

    return solid;
}

SolidCylinder * ModelSet::createCylinder(glm::mat4 mat, double div)
{
    auto solid = new SolidCylinder();
    auto meshs = file->getMeshes();

    for (auto i = 0; i < meshs.size(); i++)
    {
        for (auto& vb : file->getVertices())
        {
            auto v = glm::vec3(mat * glm::vec4(vb, 1.0));

            if (v.x < min.x)	min.x = v.x;
            if (v.y < min.y)	min.y = 0.0;
            if (v.z < min.z)	min.z = v.z;
            if (v.x > max.x)	max.x = v.x;
            if (v.y > max.y)	max.y = 0.0;
            if (v.z > max.z)	max.z = v.z;
            
        }
    }

    glm::vec3 center = (max + min) / glm::vec3(2.0, 2.0, 2.0);

    double radius = glm::distance(center, max);

    solid->setPositionVector(center);
    solid->setRadius(radius / div);

    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);

    return solid;
}

GLuint ModelSet::getSetSize()
{
    return size;
}

GLvoid ModelSet::bindModelMatrixArray()
{
    glBindBuffer(GL_ARRAY_BUFFER, matricesID);
}

void ModelSet::setMatrices(std::vector<glm::mat4> matrices)
{
    glBindVertexArray(mesh->getID());
    glGenBuffers(1, &matricesID);
    glBindBuffer(GL_ARRAY_BUFFER, matricesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * matrices.size(), matrices.data(), GL_STATIC_DRAW);
    size = matrices.size();

    for (auto z = 0; z < 4; z++) {
        glEnableVertexAttribArray(4 + z);
        glVertexAttribPointer(4 + z, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * z * 4));
        glVertexAttribDivisor(4 + z, 1);
    }
}

ModelAsset * ModelSet::getMeshes() 
{
    return file;
}

GPU_Transfer * ModelSet::getMesh() 
{
    return mesh;
}

std::vector<glm::mat4>& ModelSet::getMatrices() 
{
    return matrices;
}

std::vector<GPU_Sampler *>& ModelSet::getTextures()
{
    return samplers;
}