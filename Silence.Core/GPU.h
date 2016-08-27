
#pragma once

#include "GPU_Transfer.h"
#include "GPU_Program.h"
#include "GPU_Sampler.h"
#include "EngineLayer.h"
#include "GPU_Matrices.h"

#define TRIANGLE 3
#define CUBE 36
#define QUAD 6

enum GRAPHIC_API {
    OPENGL,
    DIRECTX
};

class SILENCE_EXPORT GPU
{
    const GLubyte * shaderVersion;
    const GLubyte * renderer;
    const GLubyte * version;
    const GLubyte * vendor;
    GLboolean grabbedGPU;
public:
    explicit GPU(GLboolean printDetails = GL_TRUE);
    ~GPU();

    GLboolean has(GRAPHIC_API api, GLdouble version);
};