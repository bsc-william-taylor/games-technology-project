
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
    direction = glm::vec3(0.0, -1.0, 0.0);
    colour = glm::vec3(1.0, 1.0, 1.0);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
    this->direction = glm::normalize(direction);
}

void DirectionalLight::setColour(glm::vec3 colour)
{
    this->colour = colour;
}

void DirectionalLight::send(GPU_Program * p)
{
    GLuint ID = p->getID();

    if (ID != NULL && directionalLightActive)
    {
        glUniform3fv(glGetUniformLocation(ID, "sceneLight.lightDirection"), 1, glm::value_ptr(direction));
        glUniform3fv(glGetUniformLocation(ID, "sceneLight.lightColour"), 1, glm::value_ptr(colour));
        glUniform1i(glGetUniformLocation(ID, "sceneLight.active"), 1);
    }

    if (ID != NULL && !directionalLightActive)
    {
        glUniform1f(glGetUniformLocation(ID, "sceneLight.active"), NULL);
    }
}

void DirectionalLight::turnOff()
{
    this->directionalLightActive = 0;
}

void DirectionalLight::turnOn()
{
    this->directionalLightActive = 1;
}