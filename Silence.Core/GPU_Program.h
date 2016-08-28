
#pragma once

#include "EngineLayer.h"

#ifndef SHADER_VERSION
    #define SHADER_VERSION "#version 330 \n" 
#else
    #error SHADER_VERSION : multiple definition
#endif

#ifndef SRC
    #define SRC(str) SHADER_VERSION #str
#else
    #error SRC : multiple definition
#endif

class SILENCE_EXPORT GPU_Program
{
    GLboolean compiled;

    GLuint fragmentID;
    GLuint programID;
    GLuint vertexID;
public:
    GPU_Program();
    ~GPU_Program();

    GLvoid setFragmentSource(const char * source);
    GLvoid setVertexSource(const char * source);
    GLvoid setMatrix(GLchar * name, GLfloat * value);
    GLvoid setVector(GLchar * name, glm::vec2 vec);
    GLvoid setFloat(GLchar * name, float v);
    GLvoid setInteger(GLchar * name, int v);
    GLvoid prepare(GLuint VAO);
    GLvoid bind(GLuint t_ID);
    GLvoid unbindShader();
    GLvoid bindShader();

    GLuint getID();

    GLvoid run(GLuint shapeType, GLuint starting_vertex, GLuint number_of_vertices);
    GLvoid run(GLuint starting_vertex, GLuint number_of_vertices);
    GLvoid run(GLuint number_of_vertices);

    GLboolean compile();
};