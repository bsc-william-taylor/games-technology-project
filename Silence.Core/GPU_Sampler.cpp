
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

#include "GPU_Sampler.h"

// Constructor & Deconstructor
GPU_Sampler::GPU_Sampler(SamplerType type)
    : samplerType(type)
{
    // set up some default sampler settings
    min_filter = GL_LINEAR;
    mag_filter = GL_LINEAR;
    hasSent = GL_FALSE;
    wrap_s = GL_REPEAT;
    wrap_t = GL_REPEAT;

    // if the sampler is a cubemap type
    if (samplerType == CUBE_SAMPLER)
    {
        // generate a cubemap ID
        glGenTextures(1, &cubemapID);
    }
    else
    {
        bitmaps.push_back(new Bitmap());
        glGenTextures(1, &bitmaps[0]->bitmapID);
    }
}

GPU_Sampler::~GPU_Sampler()
{
    // if the sampler type is a single sampler
    if (samplerType == SINGLE_SAMPLER)
    {
        // delete the single texture which would of been loaded
        glDeleteTextures(1, &bitmaps[0]->bitmapID);
        for (auto& bitmap : bitmaps) {
            delete bitmap;
        }
    }
    else
    {
        // else delete the cubemap texture
        glDeleteTextures(1, &cubemapID);
        for (auto& bitmap : bitmaps) {
            delete bitmap;
        }
    }
}

// Sends the bitmap data to the GPU
GLvoid GPU_Sampler::send()
{
    // if no data has been submitted dont do anything
    if (!bitmaps.empty())
    {
        // if the sampler type is a single sampler
        if (samplerType == SINGLE_SAMPLER)
        {
            // bind the first texture that has been loaded
            glBindTexture(GL_TEXTURE_2D, bitmaps[0]->bitmapID);
            // set the texture settings
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

            // then send the data to the gpu
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                bitmaps[0]->bitmapWidth,
                bitmaps[0]->bitmapHeight, 0,
                GL_BGRA,
                GL_UNSIGNED_BYTE,
                bitmaps[0]->data
                );

            // and generate mipmaps if the settings require it
            if (min_filter == GL_LINEAR_MIPMAP_LINEAR || min_filter == GL_LINEAR_MIPMAP_NEAREST)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }

        // if the sampler is a cubemap
        if (samplerType == CUBE_SAMPLER)
        {
            // set the order of the textures which will be loaded
            GLenum faceType[] = {
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y
            };

            // bind the cubemap texture
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
            // set the settings for the cubemap
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            // then load each texture
            for (int i = 0; i < 6; i++)  {
                glTexImage2D(faceType[i], 0, GL_RGBA,
                    bitmaps[i]->bitmapWidth,
                    bitmaps[i]->bitmapHeight, 0,
                    GL_BGRA,
                    GL_UNSIGNED_BYTE,
                    bitmaps[i]->data
                    );
            }
        }

        hasSent = GL_TRUE;
    }
}


// sets the bitmap wrapping settings
GLvoid GPU_Sampler::setBitmapWrapping(GLuint s, GLuint t)
{
    wrap_s = s;
    wrap_t = t;
}

// sets the bitmap filter settings
GLvoid GPU_Sampler::setTransferQuality(GLuint min, GLuint mag)
{
    min_filter = min;
    mag_filter = mag;
}

// returns the correct ID for the object
GLuint GPU_Sampler::getID()
{
    if (hasSent)
    {
        // if a cubemap sampler
        if (samplerType == CUBE_SAMPLER)
        {
            // return cubemap id
            return cubemapID;
        }
        else
        {
            // else return the bitmap ID
            return bitmaps[0]->bitmapID;
        }
    }
    else
    {
        return NULL;
    }
}

GLvoid GPU_Sampler::setBitmapData(GLvoid * data, GLuint w, GLuint h, GLuint bpp, GLuint bm)
{
    if (samplerType == SINGLE_SAMPLER) {
        bitmaps[0]->data = data;
        bitmaps[0]->bitmapWidth = w;
        bitmaps[0]->bitmapHeight = h;
        bitmaps[0]->bytesPerPixel = bpp;
        bitmaps[0]->bitmapMask = bm;
    } else {
        Bitmap * bitmap = new Bitmap();

        bitmap->data = data;
        bitmap->bitmapWidth = w;
        bitmap->bitmapHeight = h;
        bitmap->bytesPerPixel = bpp;
        bitmap->bitmapMask = bm;

        bitmaps.push_back(bitmap);
    }
}