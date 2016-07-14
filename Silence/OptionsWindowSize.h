
#pragma once

#include "InteractionLayer.h"

struct Resolution {
	unsigned int height;
	unsigned int width;
	unsigned int pc;

	Resolution(int w, int h) {
		height = h;
		width = w;
		pc = w*h;
	}

	std::string to_str() {
		return std::to_string(width).append("x").append(std::to_string(height));
	}
};

class OptionsWindowSize
{
private:
	LocalAssetManager * package;
	Button2D windowSizeButtons[2];
	Text2D windowSizeLabel;
	Text2D windowSize;
	Window* window;

	SDL_Color colour;
public:
	OptionsWindowSize(Window *);
	~OptionsWindowSize();

	void onGamepadButton(int key, int state, int a);
	void create(LocalAssetManager *);
	void update();
	void render(Renderer2D *);
	void event(SDL_Event&);
private:
	std::vector<Resolution*> resolutions;
	int currentResolution;
	Resolution convert(std::string text);

	void increaseResolution();
	void decreaseResolution();
};