
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT SolidBox
{
    glm::vec3 position;
    glm::vec3 slide;
    glm::vec3 size;

    bool reversed;
public:
    SolidBox(glm::vec3 position, glm::vec3 size);
    SolidBox();
    virtual ~SolidBox();

    glm::vec3 getSlideVector() const;
    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

    void setPositionVector(glm::vec3 position);
    void setSizeVector(glm::vec3 size);
    void reverse();

    bool collides(glm::vec3 position) const;
};