
#pragma once

#include "InteractionLayer.h"


class Border
{
private:
	SolidBox * borderArea;
	Matrices matrices;
	Model wall;
public:
	Border();
	~Border();

	void create(LocalAssetManager *, World& world);
	void render(ForwardRenderer&);
	void update();
private:
	void renderLeftAndRight(ForwardRenderer& renderer);
	void renderBackAndFront(ForwardRenderer& renderer);
};