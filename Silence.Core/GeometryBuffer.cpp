
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

#include "GeometryBuffer.h"

// Constructor & Deconstructor
GeometryBuffer::GeometryBuffer()
{
    // Here we just set their initial values
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
    // If the framebuffer has indeed been created we destroy all objects
    if (framebuffer != NULL) {
        glDeleteTextures(1, &positionTexture);
        glDeleteTextures(1, &normalTexture);
        glDeleteTextures(1, &finalTexture);
        glDeleteTextures(1, &depthTexture);
        glDeleteFramebuffers(1, &framebuffer);
    }
}

// This function simply sets up all the textures needed
void GeometryBuffer::setupGeometryBuffer(int w, int h)
{
    // set the width and height of the buffer
    bufferHeight = h;
    bufferWidth = w;

    // then generate our framebuffer and bind it
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // then generate our depth texture
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, bufferWidth, bufferHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);

    // Then generate our position texture to store positions
    glBindTexture(GL_TEXTURE_2D, positionTexture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB16F, bufferWidth, bufferHeight, NULL, GL_RGB, GL_FLOAT, NULL);

    // Then generate our normal texture to store normals
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB16F, bufferWidth, bufferHeight, NULL, GL_RGB, GL_FLOAT, NULL);

    // Then generate the diffuse texture which will store colour values for our objects
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB8, bufferWidth, bufferHeight, NULL, GL_RGB, GL_FLOAT, NULL);

    // The generate our final texture which is used as a secondary buffer
    glBindTexture(GL_TEXTURE_2D, finalTexture);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB8, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);

    // Then attach all these textures to the framebuffer we just created
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, diffuseTexture, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalTexture, NULL);

    // Finally check to see if the framebuffer object has been created successfully
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // if not throw an exception
    if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw Error(RENDERER, "couldnt construct framebuffer object");
    }

    // Then bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

// simply returns the size of the buffer which should match the window
glm::vec2 GeometryBuffer::getBufferSize()
{
    return glm::vec2(bufferWidth, bufferHeight);
}

// Next we prepare the buffer for reading the pixel data
void GeometryBuffer::prepareForReading()
{
    // set the buffer we draw to as the final texture object we created
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    // then bind each framebuffer texture object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, positionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
}

// Starts deferred process
void GeometryBuffer::startFrame()
{
    // By clearing the framebuffer we created
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // and setting the active texture to draw into as the final one
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
}

// Finishes the deferred procress
void GeometryBuffer::stopFrame()
{
    // Binds the final framebuffer and copies all its data into the default buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
    glBlitFramebuffer(0, 0, bufferWidth, bufferHeight, 0, 0, bufferWidth, bufferHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

// Prepares the layout of the colour attachments to write into 
void GeometryBuffer::prepareForWriting()
{
    // Bind our Geometry buffer / framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Set the layout of the attachments
    GLenum bufferInstructions[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };

    // and set these as the layout of the buffer & clear the colour buffer
    glDrawBuffers(3, bufferInstructions);
    glClear(GL_COLOR_BUFFER_BIT);
}