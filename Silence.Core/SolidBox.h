
#ifndef __SOLID_BOX_H__
#define __SOLID_BOX_H__

#include "EngineLayer.h"

class SILENCE_EXPORT SolidBox
{
private:
    glm::vec3 position;
    glm::vec3 slide;
    glm::vec3 size;

    bool reversed;
public:
    SolidBox(glm::vec3 p, glm::vec3 s);
    SolidBox();
    ~SolidBox();

    glm::vec3 getSlideVector() {
        return slide;
    }

    glm::vec3 getMin();
    glm::vec3 getMax();

    void setPositionVector(glm::vec3);
    void setSizeVector(glm::vec3);
    void reverse();

    bool collides(glm::vec3);
};

#endif