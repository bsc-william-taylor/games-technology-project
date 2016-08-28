
#pragma once

#include "EngineLayer.h"
#include "ModelFile.h"
#include "World.h"
#include "GPU.h"

class SILENCE_EXPORT Model
{
    vector<GPU_Sampler *> samplers;
    GPU_Transfer * mesh;
    ModelAsset * file;
    Vertex min, max;
public:
    Model();
    ~Model();

    SolidCylinder * createCylinder(glm::mat4, double);
    SolidBox * createbox(glm::mat4);
    
    ModelAsset * getModelAsset();
    ModelAsset * getMeshes();

    GPU_Transfer * getMesh();
  
    GLvoid setModel(ModelAsset * model);
    GLuint getMaterialID(GLuint);

    vector<GPU_Sampler *>& getTextures();
};