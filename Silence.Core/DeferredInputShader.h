
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
#include "GPU.h"

// This is our class which is the input shader for the deferred render
class SILENCE_EXPORT DeferredInputShader
{
private:
    // The LLGI GPU_Program object
    GPU_Program * program;
public:
    // Constructor & Deconstructor
    DeferredInputShader();
    ~DeferredInputShader();

    // returns a pointer to the GPU_Program object
    GPU_Program * getProgram();

    // prepares the shader ready for execution
    void prepare(int VAO, int textureID);
    // binds the VAO for drawing the object
    void prepare(int VAO);
    // compiles the shader ready for use
    void compile();

    //
    void runIndex(int ID, int numberOfIndices);
    // runs the program with the specified geometry type
    void run(int geometryType, int start_v, int end_v);
    // draw the mesh through start_v to end_v
    void run(int start_v, int end_v);
    // draw this number of meshes
    void run(int vertex_count);
};