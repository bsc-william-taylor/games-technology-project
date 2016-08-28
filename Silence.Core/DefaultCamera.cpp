
#include "DefaultCamera.h"

DefaultCamera::DefaultCamera()
    : viewMatrix(glm::mat4())
{
}

DefaultCamera::~DefaultCamera()
{
}

glm::mat4& DefaultCamera::getView()
{
    return viewMatrix;
}

void DefaultCamera::handleTerrainHeight(float f)
{
}

void DefaultCamera::cancelMovement()
{
}

glm::vec3 DefaultCamera::getPosition()
{
    return glm::vec3(0.0, 0.0, 0.0);
}

void DefaultCamera::updateCameraPosition(SDL_Event&)
{
}

void DefaultCamera::setPosition(glm::vec3)
{
}

void DefaultCamera::repositionCamera()
{
}

GLfloat DefaultCamera::getHeight()
{
    return 0.0f;
}

void DefaultCamera::prepareCamera()
{    
}

void DefaultCamera::onGamepadAxis(int, float)
{
}

glm::vec3 DefaultCamera::getCameraDirection()
{
    return glm::vec3(0.0, 0.0, 0.0);
}