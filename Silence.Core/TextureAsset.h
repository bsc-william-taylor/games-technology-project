
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT TextureAsset
{
    FIBITMAP * bitmap;
    std::string name;
public:
    explicit TextureAsset(std::string name);
    ~TextureAsset();
    
    std::string getName() const;

    void grabFromFile(const char * filename);
    void recycle();

    RGBQUAD getPixelColour(int x, int y) const;

    int getHeight() const;
    int getWidth() const;
    int getMask() const;
    int getBPP() const;

    void* getPixels() const;
};