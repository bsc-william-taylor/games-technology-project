
#include "SolidCylinder.h"

SolidCylinder::SolidCylinder()
{
    position = glm::vec3(0.0, 0.0, 0.0);
    radius = 0;
}

SolidCylinder::~SolidCylinder()
{
}

void SolidCylinder::setPositionVector(glm::vec3 position)
{
    this->position = position;
}

void SolidCylinder::setRadius(double r)
{
    radius = r;
}

bool SolidCylinder::collides(glm::vec3 vector) const
{
    return abs(glm::length(vector - position)) <= radius;
}