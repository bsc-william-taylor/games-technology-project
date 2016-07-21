
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

#include "Renderer2D.h"

// Constructor & Deconstructor
Renderer2D::Renderer2D()
{
}

Renderer2D::~Renderer2D()
{
}

// Creates the renderer ready for use
void Renderer2D::createRenderer()
{
    // here we just compile the shader
    shader.compile();
}

// renders buttons on the screen for the user
void Renderer2D::renderButtons(Button2D * buttons2D, int sz)
{
    for (int i = 0; i < sz; i++)
    {
        renderButton(&buttons2D[i]);
    }
}

// renders a texture to the screen
void Renderer2D::renderTexture(Texture2D * texture)
{

    // check to make sure the texture was created
    if (texture != NULL)
    {
        // then render the texture
        shader.prepare(texture->getDataID(), texture->getTextureID());
        shader.run(6);
    }
    else
    {
        // throws an exception if a null texture was passed
        throw Error(RENDERER, "NULL texture object was passed to the renderer", William);
    }
}

//
void Renderer2D::setAlpha(float alpha)
{
    GPU_Program * program = shader.getProgram();

    program->setFloat("alpha", alpha);
}

void Renderer2D::renderLabel(Text2D* text)
{
    // check to make sure the texture was created
    if (text != NULL)
    {
        // then render the label
        shader.prepare(text->getDataID(), text->getTextureID());
        shader.run(6);
    }
    else
    {
        // throws an exception if a null text was passed
        throw Error(RENDERER, "NULL text object was passed to the renderer", William);
    }
}

// sets the matrix for the shader
void Renderer2D::setMatrixForObject(const char * name, glm::mat4& mat)
{
    // grab the raw GPU_Program
    GPU_Program * program = shader.getProgram();
    // and set the matrix value
    program->setMatrix((char *)name, glm::value_ptr(mat));
}

void Renderer2D::renderButton(Button2D* button)
{
    // get the texture and label
    Texture2D * buttonTexture = button->getTexture();
    Text2D * buttonLabel = button->getText();

    // render the texture
    if (buttonTexture != NULL) {
        renderTexture(buttonTexture);
    }

    // and the label
    if (buttonLabel != NULL) {
        renderLabel(buttonLabel);
    }
}

// prepare the render for rendering 2D object
void Renderer2D::prepare()
{
    // enable blend for the text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // disable the depth test
    glDisable(GL_DEPTH_TEST);

    // bind the 2D shader to render objects
    shader.getProgram()->bindShader();
}

void Renderer2D::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::present()
{
    // unbind the 2D shader as we no longer need to
    shader.getProgram()->unbindShader();
    
    glDisable(GL_BLEND);
}