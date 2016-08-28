
#include "Lights.h"

Lights::Lights()
{
    directionalLight = nullptr;
    spotLight = nullptr;
}

Lights::~Lights()
{
}

void Lights::pushDirectionalLight(DirectionalLight * directionalLight)
{
    this->directionalLight = directionalLight;
}

void Lights::pushPointLight(PointLight * pointLight)
{
    pointLights.push_back(pointLight);
}

void Lights::pushSpotLight(SpotLight * spot)
{
    this->spotLight = spot;
} 

void Lights::sendLights(glm::vec3 cameraPosition, glm::vec3 cameraDirection, GPU_Program * p)
{
    if (directionalLight != nullptr) 
    {
        directionalLight->send(p);
    }

    if (spotLight != nullptr) 
    {
        spotLight->setPosition(cameraPosition);
        spotLight->setDirection(cameraDirection);
        spotLight->send(p);
    }

    for (auto i = 0; i < pointLights.size(); i++) 
    {
        pointLights[i]->send(p, i);
    }
}