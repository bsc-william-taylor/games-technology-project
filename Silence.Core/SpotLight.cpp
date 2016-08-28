
#include "SpotLight.h"

SpotLight::SpotLight() :
    spotLightActive(true),
    linearAttrib(0.0),
    coneCosine(0.0)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::setDirection(glm::vec3 direction)
{
    spotDirection = glm::normalize(direction);
}

void SpotLight::setPosition(glm::vec3 position)
{
    spotPosition = position;
}

void SpotLight::setColour(glm::vec3 colour)
{
    spotColour = colour;
}

void SpotLight::setAttribuation(float attr)
{
    linearAttrib = attr;
}

void SpotLight::setConeAngle(float angle)
{
    coneCosine = cos(angle * 3.1415 / 180.0);
}

void SpotLight::send(GPU_Program * p)
{
    auto ID = p->getID();

    if (ID != NULL && spotLightActive)
    {
        glUniform3fv(glGetUniformLocation(ID, "spotLight.spotDirection"), 1, glm::value_ptr(spotDirection));
        glUniform3fv(glGetUniformLocation(ID, "spotLight.spotPosition"), 1, glm::value_ptr(spotPosition));
        glUniform3fv(glGetUniformLocation(ID, "spotLight.spotColour"), 1, glm::value_ptr(spotColour));
        glUniform1f(glGetUniformLocation(ID, "spotLight.linearAttrib"), linearAttrib);
        glUniform1f(glGetUniformLocation(ID, "spotLight.coneCosine"), coneCosine);
        glUniform1i(glGetUniformLocation(ID, "spotLight.active"), 1);
    }

    if (ID != NULL && !spotLightActive)
    {
        glUniform3fv(glGetUniformLocation(ID, "spotLight.spotDirection"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
        glUniform1f(glGetUniformLocation(ID, "spotLight.coneCosine"), 0.0);
        glUniform1i(glGetUniformLocation(ID, "spotLight.active"), 0);
    }
}

void SpotLight::toggle()
{
    spotLightActive = !spotLightActive;
}

void SpotLight::turnOff()
{
    spotLightActive = false;
}

void SpotLight::turnOn()
{
    spotLightActive = true;
}