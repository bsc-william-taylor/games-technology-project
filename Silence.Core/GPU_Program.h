
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

// Make sure we dont have a definition already it will cause major problems
#ifndef SHADER_VERSION
    #define SHADER_VERSION "#version 330 \n" 
#else
    #error SHADER_VERSION : multiple definition
#endif

// It also worth noting that these variables shouldnt change
#ifndef SRC
    #define SRC(str) SHADER_VERSION #str
#else
    #error SRC : multiple definition
#endif

// Here is a class that compiles and load a GPU program
class SILENCE_EXPORT GPU_Program
{
private:
    // has the program been compiled
    GLboolean compiled;
    // The ID for the fragment shader
    GLuint fragmentID;
    // The ID for the program itself
    GLuint programID;
    // The ID for the vertex shader
    GLuint vertexID;
public:
    // Constructor & Deconstructor
    GPU_Program();
    ~GPU_Program();

    // sets the source code for the fragment shader
    GLvoid setFragmentSource(const char * source);
    // sets the source code for the vertex shader
    GLvoid setVertexSource(const char * source);
    // sets a matrix value for the program
    GLvoid setMatrix(GLchar * name, GLfloat * value);
    // sets a vector value inside the shader
    GLvoid setVector(GLchar * name, glm::vec2 vec);
    //
    GLvoid setFloat(GLchar * name, float v);
    // sets a interger inside the shader
    GLvoid setInteger(GLchar * name, int v);
    // prepare the program to be run on the VAO
    GLvoid prepare(GLuint VAO);
    // binds a texture to the current program
    GLvoid bind(GLuint t_ID);
    // unbinds the shader
    GLvoid unbindShader();
    // binds the shader
    GLvoid bindShader();
    // returns the ID for the program
    GLuint getID();

    // Makes a draw call 
    GLvoid run(GLuint shapeType, GLuint starting_vertex, GLuint number_of_vertices);
    GLvoid run(GLuint starting_vertex, GLuint number_of_vertices);
    GLvoid run(GLuint number_of_vertices);

    // compiles the program
    GLboolean compile();
};