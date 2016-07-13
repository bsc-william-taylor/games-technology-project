
#pragma once

#include "EngineLayer.h"
#include "md2Model.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "Ai.h"
#include "Intelligence.h"

class Monster
{
private:
	Intelligence intelligence;
	AnimatedModel * enemy;
	Matrices matrices;
	bool updated;
	SolidBox * box;
	float alpha;
public:
	Monster();
	~Monster();

	bool hasKilledPlayer(glm::vec3);
	bool isCloseToPlayer(glm::vec3);

	bool isActive();

	void create(LocalAssetManager * package);
	void update(Camera *);
	void render(ForwardRenderer& renderer);
	void distract(glm::vec3);
	void reset();

	float getTravel() {
		return intelligence.getTravel();
	}

	glm::vec3 getPosition();
private:
	float enemyDirection;
	glm::vec3 position;
private:
	glm::vec3 to_scene_space(glm::vec3);
	glm::vec3 cameraPosition;

	int spawnID;
};