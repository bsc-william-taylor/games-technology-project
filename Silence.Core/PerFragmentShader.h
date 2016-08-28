
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT PerFragmentShader
{
    GPU_Program * program;
public:
    PerFragmentShader();
    ~PerFragmentShader();

    GPU_Program * getProgram();

    void compile();
    void prepare(int VAO, int textureID);
    void prepare(int VAO);
    void bind(std::string name, int ID, int value);
    void runIndex(int ID, int numberOfIndices);
    void run(int geometryType, int start_v, int end_v);
    void run(int start_v, int end_v);
    void run(int vertex_count);
};