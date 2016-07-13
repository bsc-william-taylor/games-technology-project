
#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Lights
{
private:
	DirectionalLight * directionalLight;
	SpotLight * spotLight;

	std::vector<PointLight *> pointLights;
public:
	Lights();
	~Lights();

	void pushDirectionalLight(DirectionalLight * );
	void pushPointLight(PointLight *);
	void pushSpotLight(SpotLight *);
	void sendLights(glm::vec3 p, glm::vec3 , GPU_Program *);
};