
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT SolidCylinder
{
    glm::vec3 position;
    double radius;
public:
    SolidCylinder();
    virtual ~SolidCylinder();

    bool collides(glm::vec3 position) const;

    void setPositionVector(glm::vec3 position);
    void setRadius(double areaRadious);
};