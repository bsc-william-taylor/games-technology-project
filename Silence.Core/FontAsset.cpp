
#include "FontAsset.h"

FontAsset::FontAsset(std::string name) :
    colour({ 255, 255, 255}),
    font(nullptr),
    name(name),
    text(name),
    sz(0)
{
}

FontAsset::~FontAsset()
{
    if (font != nullptr) 
    {
        TTF_CloseFont(font);
    }
}

SDL_Surface * FontAsset::generateTexture()
{
    SDL_Surface * fontSurface = nullptr;

    if (font == nullptr || sz != -1) 
    {
        font = TTF_OpenFont(filename.c_str(), sz);
        sz = -1;
    }

    if (font) 
    {
        fontSurface = TTF_RenderText_Blended(font, text.c_str(), colour);
        TTF_CloseFont(font);
        font = nullptr;
    }

    return fontSurface;
}

void FontAsset::setColour(SDL_Color c) 
{
    colour = c;
}

void FontAsset::setText(const char * t)
{
    text = t;
}

void FontAsset::grabFromFile(std::string font)
{
    filename = font;
}

void FontAsset::openAtSize(int sz)
{
    this->sz = sz;
}

std::string FontAsset::getName() const
{
    return name;
}