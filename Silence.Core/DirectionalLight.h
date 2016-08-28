
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT DirectionalLight 
{
    bool directionalLightActive;
    glm::vec3 direction;
    glm::vec3 colour;
public:
    DirectionalLight();
    ~DirectionalLight();

    void setDirection(glm::vec3 direction);
    void setColour(glm::vec3 colour);
    void send(GPU_Program * p);
    void turnOff();
    void turnOn();
};