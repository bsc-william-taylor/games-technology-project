
#include "GPU_Sampler.h"

GPU_Sampler::GPU_Sampler(SamplerType type)
    : samplerType(type)
{
    min_filter = GL_LINEAR;
    mag_filter = GL_LINEAR;
    hasSent = GL_FALSE;
    wrap_s = GL_REPEAT;
    wrap_t = GL_REPEAT;

    if (samplerType == CUBE_SAMPLER)
    {
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
    if (samplerType == SINGLE_SAMPLER)
    {
        glDeleteTextures(1, &bitmaps[0]->bitmapID);
    }
    else
    {
        glDeleteTextures(1, &cubemapID);
    }

    for (auto& bitmap : bitmaps)
    {
        delete bitmap;
    }
}

GLvoid GPU_Sampler::send()
{
    if (!bitmaps.empty())
    {
        if (samplerType == SINGLE_SAMPLER)
        {
            glBindTexture(GL_TEXTURE_2D, bitmaps[0]->bitmapID);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

            auto height = bitmaps[0]->bitmapHeight;
            auto width = bitmaps[0]->bitmapWidth;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmaps[0]->data);

            if (min_filter == GL_LINEAR_MIPMAP_LINEAR || min_filter == GL_LINEAR_MIPMAP_NEAREST)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }

        if (samplerType == CUBE_SAMPLER)
        {
            GLenum faceType[] = {
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y
            };

            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            for (auto i = 0; i < 6; i++)
            {
                auto height = bitmaps[i]->bitmapHeight;
                auto width = bitmaps[i]->bitmapWidth;

                glTexImage2D(faceType[i], 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmaps[i]->data);
            }
        }

        hasSent = GL_TRUE;
    }
}

GLvoid GPU_Sampler::setBitmapWrapping(GLuint s, GLuint t)
{
    wrap_s = s;
    wrap_t = t;
}

GLvoid GPU_Sampler::setTransferQuality(GLuint min, GLuint mag)
{
    min_filter = min;
    mag_filter = mag;
}

GLuint GPU_Sampler::getID()
{
    if (hasSent)
    {   
        return samplerType == CUBE_SAMPLER ? cubemapID : bitmaps[0]->bitmapID;
    }
    
    return NULL;
}

GLvoid GPU_Sampler::setBitmapData(GLvoid * data, GLuint w, GLuint h, GLuint bpp, GLuint bm)
{
    if (samplerType == SINGLE_SAMPLER) 
    {
        bitmaps[0]->data = data;
        bitmaps[0]->bitmapWidth = w;
        bitmaps[0]->bitmapHeight = h;
        bitmaps[0]->bytesPerPixel = bpp;
        bitmaps[0]->bitmapMask = bm;
    } 
    else 
    {
        const auto bitmap = new Bitmap();
        bitmap->data = data;
        bitmap->bitmapWidth = w;
        bitmap->bitmapHeight = h;
        bitmap->bytesPerPixel = bpp;
        bitmap->bitmapMask = bm;
        bitmaps.push_back(bitmap);
    }
}