
#pragma once

#include "EngineLayer.h"
#include "HLGI.h"
#include "ForwardRenderer.h"
#include "LocalAssetManager.h"
#include "Music.h"

class Key
{
private:
	Matrices matrices;
	PointLight * point;
	Model * keyModel;
	Music pickup;
	int spawnID;

	bool taken;
public:
	Key();
	~Key();

	glm::vec3 getPosition();

	void create(Model * key, LocalAssetManager * manager);
	void spawn(World& world, PointLight * p);
	void update();
	void render(ForwardRenderer&);
	void reset();
	
	bool hasBeenPickedUp();
};