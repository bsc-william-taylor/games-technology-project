
#include "TextureAsset.h"

TextureAsset::TextureAsset(std::string n) :
    bitmap(nullptr),
    name(n)
{
}

TextureAsset::~TextureAsset()
{
    if (bitmap != nullptr)
    {
        FreeImage_Unload(bitmap);
    }
}

void TextureAsset::grabFromFile(const char * filename)
{
    auto loadedBitmap = FreeImage_Load(FreeImage_GetFIFFromFilename(filename), filename);

    if (loadedBitmap == nullptr)
    {
        throw Error(Component::FileIO, std::string(filename, " couldnt load a texture"), Author::William);
    }

    bitmap = FreeImage_ConvertTo32Bits(loadedBitmap);
    
    FreeImage_Unload(loadedBitmap);
}

void TextureAsset::recycle()
{
    if (bitmap != nullptr)
    {
        FreeImage_Unload(bitmap);
        bitmap = nullptr;
    }
}

RGBQUAD TextureAsset::getPixelColour(int x, int y) const
{
    RGBQUAD quad;
    FreeImage_GetPixelColor(bitmap, x, y, &quad);
    return quad;
}

std::string TextureAsset::getName() const
{
    return name;
}

void * TextureAsset::getPixels() const
{
    return FreeImage_GetBits(bitmap);
}

int TextureAsset::getBPP() const
{
    return FreeImage_GetBPP(bitmap);
}

int TextureAsset::getMask() const
{
    return FreeImage_GetRedMask(bitmap);
}

int TextureAsset::getWidth() const
{
    return FreeImage_GetWidth(bitmap);
}

int TextureAsset::getHeight() const
{
    return FreeImage_GetHeight(bitmap);
}