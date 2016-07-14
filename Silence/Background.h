

#pragma once

#include "InteractionLayer.h"

class Background 
{
private:
	LocalAssetManager * package;
	HighPrecisionTimer timer;
	Texture2D background;
	unsigned int index;
	float alpha;
public:
	Background();
	~Background();

	void create(LocalAssetManager *);
	void render(Renderer2D *);
	void event(SDL_Event&);
	void update();
	void enter();

	float getAlpha();
};