
#include "GeometryBuffer.h"

GeometryBuffer::GeometryBuffer() :
    bufferHeight(0), bufferWidth(0)
{
    positionTexture = NULL;
    diffuseTexture = NULL;
    normalTexture = NULL;
    depthTexture = NULL;
    finalTexture = NULL;
    framebuffer = NULL;

    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &depthTexture);
    glGenTextures(1, &positionTexture);
    glGenTextures(1, &normalTexture);
    glGenTextures(1, &diffuseTexture);
    glGenTextures(1, &finalTexture);
}

GeometryBuffer::~GeometryBuffer()
{
    if (framebuffer != NULL) 
    {
        glDeleteTextures(1, &positionTexture);
        glDeleteTextures(1, &normalTexture);
        glDeleteTextures(1, &finalTexture);
        glDeleteTextures(1, &depthTexture);
        glDeleteFramebuffers(1, &framebuffer);
    }
}

void GeometryBuffer::setupGeometryBuffer(int w, int h)
{
    bufferHeight = h;
    bufferWidth = w;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, bufferWidth, bufferHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr);

    glBindTexture(GL_TEXTURE_2D, positionTexture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB16F, bufferWidth, bufferHeight, NULL, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB16F, bufferWidth, bufferHeight, NULL, GL_RGB, GL_FLOAT, nullptr);

    // Then generate the diffuse texture which will store colour values for our objects
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB8, bufferWidth, bufferHeight, NULL, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, finalTexture);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB8, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, diffuseTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalTexture, NULL);

    auto Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw Error(Component::Renderer, "couldnt construct framebuffer object");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

glm::vec2 GeometryBuffer::getBufferSize()
{
    return glm::vec2(bufferWidth, bufferHeight);
}

void GeometryBuffer::prepareForReading()
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, positionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
}

void GeometryBuffer::startFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
}

void GeometryBuffer::stopFrame()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
    glBlitFramebuffer(0, 0, bufferWidth, bufferHeight, 0, 0, bufferWidth, bufferHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void GeometryBuffer::prepareForWriting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLenum bufferInstructions[] = 
    {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };

    glDrawBuffers(3, bufferInstructions);
    glClear(GL_COLOR_BUFFER_BIT);
}