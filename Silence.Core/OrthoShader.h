
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

// This class represents the ortho shader used to render 2D objects
class SILENCE_EXPORT OrthoShader
{
private: 
    // The LLGI program object which makes setting up a shader easier
    GPU_Program * program;
public:
    // Constructor & Deconstructor
    OrthoShader();
    ~OrthoShader();

    // This function returns the raw LLGI object
    GPU_Program * getProgram();

    // Binds the VAO and the Texture ready for a draw call or run calls
    void prepare(int VAO_ID, int textureID);
    // Compiles the shader after this call it is ready to be used
    void compile();
    // Begins a draw with num being the number of vertices to render
    void run(int num);
};