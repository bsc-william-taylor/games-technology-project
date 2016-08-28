
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT DeferredOutputShader
{
    GPU_Program * program;
    GLuint UBO;
public:
    DeferredOutputShader();
    ~DeferredOutputShader();

    void prepare(int ID);
    void compile();

    GPU_Program * getProgram();

    void runIndex(int ID, int numberOfIndices);
    void run(int v_start, int v_end);
    void run(int vertex_num);
};