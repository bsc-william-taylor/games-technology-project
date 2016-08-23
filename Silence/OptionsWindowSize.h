
#pragma once

#include "InteractionLayer.h"

struct Resolution {
    int pixels;
    int height;
    int width;

    Resolution(int w, int h) : pixels(w*h), height(h), width(w)
    {
    }

    std::string to_str() const {
        return std::to_string(width).append("x").append(std::to_string(height));
    }
};

class OptionsWindowSize
{
    LocalAssetManager * package;
    Button2D windowSizeButtons[2];
    Text2D windowSizeLabel;
    Text2D windowSize;

    vector<Resolution*> resolutions;
    int currentResolution;
    SDL_Color colour;
    Window* window;
public:
    explicit OptionsWindowSize(Window *);
    ~OptionsWindowSize();

    void onGamepadButton(int key, int state, int a);
    void create(LocalAssetManager *);
    void update();
    void render(Renderer2D *);
    void event(SDL_Event&);
private:
    Resolution convert(std::string text);

    void increaseResolution();
    void decreaseResolution();
};