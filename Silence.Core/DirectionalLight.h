
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT DirectionalLight {
	glm::vec3 direction;
	glm::vec3 colour;

	int directionalLightActive;
public:
	DirectionalLight();
	~DirectionalLight();

	void setDirection(glm::vec3 direction);
	void setColour(glm::vec3 colour);
	void send(GPU_Program * p);
	void turnOff();
	void turnOn();
};