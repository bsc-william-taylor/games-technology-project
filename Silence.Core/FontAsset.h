
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT FontAsset
{
    std::string filename;
    std::string text;
    std::string name;

    SDL_Color colour;
    TTF_Font * font;
    unsigned int sz;
public:
    explicit FontAsset(std::string name);
    ~FontAsset();

    SDL_Surface * generateTexture();

    void grabFromFile(std::string);
    void setColour(SDL_Color c);
    void setText(const char *);
    void openAtSize(int);

    std::string getName() const;
};