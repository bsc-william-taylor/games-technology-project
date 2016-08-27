
#include "SolidBox.h"

SolidBox::SolidBox(glm::vec3 p, glm::vec3 s)
{
    this->position = p;
    this->size = s;

    reversed = false;
}

SolidBox::SolidBox()
{
    reversed = false;
}

SolidBox::~SolidBox()
{
}

glm::vec3 SolidBox::getMin() const
{
    return position;
}

glm::vec3 SolidBox::getMax() const
{
    return size;
}

bool SolidBox::collides(glm::vec3 vec) const
{
    if (!reversed) 
    {
        if (vec.x >= position.x && vec.x <= size.x) 
        {
            //if (vec.y >= position.y && vec.y <= size.y)
            if (vec.z >= position.z && vec.z <= size.z) 
            {
                return true;
            }
        }

        return false;
    }
    
    if (vec.x >= position.x && vec.x <= size.x) 
    {
        //if (vec.y >= position.y && vec.y <= size.y)
        if (vec.z >= position.z && vec.z <= size.z) 
        {
            return false;
        }
    }

    return true;
}

void SolidBox::setPositionVector(glm::vec3 p)
{
    this->position = glm::vec3(glm::vec4(p, 1.0));
}

void SolidBox::setSizeVector(glm::vec3 s)
{
    this->size = glm::vec3(glm::vec4(s, 1.0));
}

void SolidBox::reverse()
{
    reversed = true;
}

glm::vec3 SolidBox::getSlideVector() const
{
    return slide;
}