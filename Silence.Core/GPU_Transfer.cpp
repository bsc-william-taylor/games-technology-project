
#include "GPU_Transfer.h"

const int GpuBuffers = 6;
const int VertexID = 0;
const int NormalID = 1;
const int TextureID = 2;
const int ColourID = 3;
const int IndiceID = 4;

GPU_Transfer::GPU_Transfer() : 
    hasSent(GL_FALSE),
    indexListSize(0),
    indices(nullptr)
{
    glGenVertexArrays(1, &vertexArrayObject);

    for (auto i = 0; i < GpuBuffers; i++)
    {
        glGenBuffers(1, &objectIDs[i]);
        objectData[i] = nullptr;
    }
}

GPU_Transfer::~GPU_Transfer()
{
    glDeleteVertexArrays(1, &vertexArrayObject);
    
    for (auto i = 0; i < GpuBuffers; i++)
    {
        objectIDs[i] ? glDeleteBuffers(1, &objectIDs[i]) : NULL;
    }
}

GLuint GPU_Transfer::getID()
{   
    return hasSent ? vertexArrayObject : 0;
}

GLuint GPU_Transfer::getIndexListSize()
{
    return indexListSize;
}

GLuint GPU_Transfer::getIndexID()
{
    return objectIDs[IndiceID];
}

void GPU_Transfer::send() 
{
    glBindVertexArray(vertexArrayObject);
    hasSent = GL_TRUE;
    
    if (objectData[VertexID] != nullptr)
    {
        glBindBuffer(GL_ARRAY_BUFFER, objectIDs[VertexID]);
        glBufferData(GL_ARRAY_BUFFER, (*objectData[VertexID]).size() * (sizeof(GLfloat) * 3), (*objectData[VertexID]).data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(VertexID);
        glVertexAttribPointer(VertexID, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        objectData[VertexID] = nullptr;
    }

    if (objectData[NormalID] != nullptr)
    {
        glBindBuffer(GL_ARRAY_BUFFER, objectIDs[NormalID]);
        glBufferData(GL_ARRAY_BUFFER, (*objectData[NormalID]).size() * (sizeof(GLfloat) * 3), (*objectData[NormalID]).data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(NormalID);
        glVertexAttribPointer(NormalID, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        objectData[NormalID] = nullptr;
    }

    if (objectData[TextureID] != nullptr)
    {
        glBindBuffer(GL_ARRAY_BUFFER, objectIDs[TextureID]);
        glBufferData(GL_ARRAY_BUFFER, (*objectData[TextureID]).size() * (sizeof(GLfloat) * 3), (*objectData[TextureID]).data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(TextureID);
        glVertexAttribPointer(TextureID, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        objectData[TextureID] = nullptr;
    }

    if (objectData[ColourID] != nullptr)
    {
        glBindBuffer(GL_ARRAY_BUFFER, objectIDs[ColourID]);
        glBufferData(GL_ARRAY_BUFFER,  (*objectData[ColourID]).size() * (sizeof(GLfloat)*3),  (*objectData[ColourID]).data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(ColourID);
        glVertexAttribPointer(ColourID, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        objectData[ColourID] = nullptr;
    }

    if (indices != nullptr)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectIDs[IndiceID]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*indices).size() * sizeof(GLuint), &(*indices)[0], GL_STATIC_DRAW);

        indexListSize = (*indices).size();
        indices = nullptr;
    }

    glBindVertexArray(NULL);
}

void GPU_Transfer::setVerticesColour(Vertices& data)
{
    objectData[ColourID] = &data;
}

void GPU_Transfer::setTextureCords(Vertices& data)
{
    objectData[TextureID] = &data;
}

void GPU_Transfer::setVertices(Vertices& data)
{
    objectData[VertexID] = &data;
}

GLvoid GPU_Transfer::setIndices(Indices& data)
{
    indices = &data;
}

void GPU_Transfer::setNormals(Vertices& data)
{
    objectData[NormalID] = &data;
}