
#ifndef __CYLINDER_BOX_H__
#define __CYLINDER_BOX_H__

#include "EngineLayer.h"

class SILENCE_EXPORT SolidCylinder
{
private:
	glm::vec3 position;
	double radius;
public:
	SolidCylinder();
	~SolidCylinder();

	void setPositionVector(glm::vec3);
	void setRadius(double);

	bool collides(glm::vec3);
};

#endif