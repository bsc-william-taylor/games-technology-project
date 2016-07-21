
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

#include "OrthoShader.h"
#include "Text2D.h"
#include "Texture2D.h"
#include "Button2D.h"

// Our 2D renderer object which will render each object
class SILENCE_EXPORT Renderer2D
{
private:
    // The 2D render object
    OrthoShader shader;
public:
    // Constructor & Deconstructor
    Renderer2D();
    ~Renderer2D();
    
    // Set a matrices value inside the shader
    void setMatrixForObject(const char * matrix, glm::mat4& value);
    // renders texture 2D on the screen for the user
    void renderTexture(Texture2D * texture2D);
    // renders buttons on the screen for the user
    void renderButtons(Button2D * button2D, int sz);
    // renders button 2D on the screen for the user
    void renderButton(Button2D * button2D);
    // renders label 2D on the screen for the user
    void renderLabel(Text2D * text2D);
    // create the renderer ready for use
    void createRenderer();
    //
    void setAlpha(float);
    // prepares the renderer for rendering objects
    void prepare();
    // presents the final object
    void present();

    void clear();
};