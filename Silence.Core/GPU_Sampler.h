
#pragma once

#include "EngineLayer.h"

enum SamplerType { SINGLE_SAMPLER, CUBE_SAMPLER };

struct Bitmap 
{
    GLenum texture_format;		
    GLuint bytesPerPixel;	
    GLuint bitmapWidth;	
    GLuint bitmapHeight;
    GLuint bitmapMask;
    GLuint bitmapID;	

    GLvoid * data;			
};

class SILENCE_EXPORT GPU_Sampler
{
    std::vector<Bitmap *>  bitmaps;

    GLboolean hasSent;
    GLuint cubemapID;
    GLuint min_filter;
    GLuint mag_filter;
    GLuint wrap_s;
    GLuint wrap_t;

    SamplerType samplerType;
public:
    explicit GPU_Sampler(SamplerType type);
    ~GPU_Sampler();

    GLvoid setBitmapData(GLvoid*, GLuint, GLuint, GLuint, GLuint);
    GLvoid setBitmapWrapping(GLuint s, GLuint t);
    GLvoid setTransferQuality(GLuint, GLuint);
    GLvoid send();
    GLuint getID();
};