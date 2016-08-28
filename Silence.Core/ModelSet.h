
#pragma once

#include "EngineLayer.h"
#include "ModelFile.h"
#include "World.h"
#include "GPU.h"

class SILENCE_EXPORT ModelSet
{
    vector<GPU_Sampler *> samplers;
    vector<glm::mat4> matrices;
    ModelAsset * file;
    GPU_Transfer * mesh;
    GpuID matricesID;
    Vertex min, max;
    int indices;
    int size;
public:
    ModelSet();
    ~ModelSet();

    GLuint getMaterialID(GLuint);
    GLuint getSetSize();
    GLuint getIndices();
   
    GPU_Transfer * getMesh();

    GLvoid bindModelMatrixArray();
    GLvoid setModel(ModelAsset * model);
    GLvoid setMatrices(std::vector<glm::mat4> matrices);
   
    SolidCylinder * createCylinder(glm::mat4, double);
    SolidBox * createbox(glm::mat4);

    vector<glm::mat4>& getMatrices();
    vector<GPU_Sampler *>& getTextures();

    ModelAsset * getModelAsset();
    ModelAsset * getMeshes();
};