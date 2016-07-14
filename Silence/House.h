
#pragma once

#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "EngineLayer.h"
#include "HLGI.h"

class House
{
private:
	Matrices matrices;
	Model house;
public:
	House();
	~House();

	void create(LocalAssetManager * manager);
	void render(ForwardRenderer& renderer);
	void update();
};