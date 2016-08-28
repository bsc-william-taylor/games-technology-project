
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT DeferredInputShader
{
    GPU_Program * program;
public:
    DeferredInputShader();
    ~DeferredInputShader();

    GPU_Program * getProgram();

    void prepare(int VAO, int textureID);
    void prepare(int VAO);
    void compile();

    void runIndex(int ID, int numberOfIndices);
    void run(int geometryType, int start_v, int end_v);
    void run(int start_v, int end_v);
    void run(int vertex_count);
};