
#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class SILENCE_EXPORT Lights
{
    std::vector<PointLight *> pointLights;
    DirectionalLight * directionalLight;
    SpotLight * spotLight;
public:
    Lights();
    ~Lights();

    void pushDirectionalLight(DirectionalLight * );
    void pushPointLight(PointLight *);
    void pushSpotLight(SpotLight *);
    void sendLights(glm::vec3 p, glm::vec3 , GPU_Program *);
};