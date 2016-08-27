
#pragma once

#include "EngineLayer.h"
#include "math.h"

using Vertices = std::vector<glm::vec3>;
using Indices = std::vector<GLuint>;
using Vertex = glm::vec3;
using GpuID = GLuint;

class SILENCE_EXPORT GPU_Transfer
{
    GLboolean hasSent;

    GLuint indexListSize;
    GLuint vertexArrayObject;
    GLuint objectIDs[6];

    Vertices * objectData[6];
    Indices * indices;
public:
    GPU_Transfer();
    ~GPU_Transfer();

    GLvoid setVerticesColour(Vertices& colour);
    GLvoid setTextureCords(Vertices& coords);
    GLvoid setVertices(Vertices& vertices);
    GLvoid setNormals(Vertices& normals);
    GLvoid setIndices(Indices& indices);
    GLvoid send();

    GLuint getIndexListSize();
    GLuint getIndexID();
    GLuint getID();
};