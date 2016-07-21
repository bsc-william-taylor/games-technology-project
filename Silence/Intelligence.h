
#pragma once


#include "EngineLayer.h"
#include "md2Model.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "Ai.h"


class Intelligence 
{
private:
    glm::vec3 tempDest;
    A_StarMap * map;
    double direction; 
    double travel;
public:
    Intelligence();
    ~Intelligence();

    double getTravel();
    double getDirection();

    glm::vec3 to_scene_space(glm::vec3);

    void setTemporaryDestination(vec3, vec3);
    bool applyUpdate(glm::vec3& position, FirstPersonCamera * camera);
    void setup(LocalAssetManager * manager);
    void reset();
    void end();
};