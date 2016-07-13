
#include "EngineLayer.h"
#include "GPU.h"


class PointLight 
{
private:
	glm::vec3 pointPosition;
	glm::vec3 pointColour;
	glm::vec4 att;

	bool pointLightActive;
public:
	PointLight();
	~PointLight();

	void setPosition(glm::vec3 position);
	void setColour(glm::vec3 colour);
	void setAttribuation(glm::vec4);

	void send(GPU_Program * p, int);
	void toggle();
	void turnOff();
	void turnOn();
};


