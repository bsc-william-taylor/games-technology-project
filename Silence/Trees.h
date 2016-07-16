
#pragma once

#include "InteractionLayer.h"
#include "ModelSet.h"

class Trees
{
private:
	ModelSet treeModels;
	std::vector<glm::mat4> matrices;
public:
	Trees();
	~Trees();

	void setup(LocalAssetManager *, Heightmap * map, World& world);
	void render(ForwardRenderer&);
};