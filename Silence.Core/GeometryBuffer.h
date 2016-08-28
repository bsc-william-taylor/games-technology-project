
#pragma once

#include "EngineLayer.h"
#include "GPU.h"

enum GeometryBufferElements 
{
    GeometryBufferElementsPosition,
    GeometryBufferElementsDiffuse,
    GeometryBufferElementsNormal,
};

class SILENCE_EXPORT GeometryBuffer
{
    GLuint positionTexture;
    GLuint diffuseTexture;
    GLuint normalTexture;
    GLuint finalTexture;
    GLuint bufferHeight;
    GLuint bufferWidth;
    GLuint depthTexture;
    GLuint framebuffer;
public:
    GeometryBuffer();
    ~GeometryBuffer();

    GLvoid setupGeometryBuffer(int w, int h);
    GLvoid prepareForReading();
    GLvoid prepareForWriting();
    GLvoid startFrame();
    GLvoid stopFrame();

    glm::vec2 getBufferSize();
};