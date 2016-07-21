
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "EngineLayer.h"

// the sampler type is it a single sampler or a cubemap
enum SamplerType {
    SINGLE_SAMPLER,
    CUBE_SAMPLER
};

// A struct for the info needed for each texture
struct Bitmap {
    GLenum texture_format;	// the format of the texture
    GLuint bitmapHeight;	// the height of the texture
    GLuint bitmapMask;		// the mask (is alpha included)
    GLuint bytesPerPixel;	// bytes per pixel in the image
    GLuint bitmapWidth;		// the width of the bitmap
    GLuint bitmapID;		// the ID for the object
    GLvoid * data;			// a pointer to pixel data
};

// A class which manages sampler/texture objects
class SILENCE_EXPORT GPU_Sampler
{
private:
    // A list of the bitmaps given to the sampler
    std::vector<Bitmap *>  bitmaps;
    // the sampler type
    SamplerType samplerType;
    // the id if any for if the sampler if a cubemap
    GLuint cubemapID;
    // Has data been sent
    GLboolean hasSent;
public:
    // Constructor & Deconstructor
    GPU_Sampler(SamplerType type);
    ~GPU_Sampler();

    // inserts some new bitmap data
    GLvoid setBitmapData(GLvoid*, GLuint, GLuint, GLuint, GLuint);
    // sets the wrapping for the bitmaps being loaded
    GLvoid setBitmapWrapping(GLuint s, GLuint t);
    // sets the quality of the sampling
    GLvoid setTransferQuality(GLuint, GLuint);
    // sends bitmap data to the gpu for us
    GLvoid send();
    // returns the ID for the textures loaded
    GLuint getID();
private:
    // the settings for the minification filter
    GLuint min_filter;
    // the settings for the magnification filter
    GLuint mag_filter;
    // the settings for wrapping the texture
    GLuint wrap_s;
    GLuint wrap_t;
};