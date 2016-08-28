
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT OrthoShader
{
    GPU_Program * program;
public:
    OrthoShader();
    ~OrthoShader();

    GPU_Program * getProgram();

    void run(int num);
    void prepare(int VAO_ID, int textureID);
    void compile();
};