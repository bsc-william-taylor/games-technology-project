
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

#ifndef __TEXT_2D_H_
#define __TEXT_2D_H_

#include "EngineLayer.h"
#include "FontAsset.h"
#include "math.h"
#include "GPU.h"

// Types of alignment for the text
enum class ALIGNMENT {
    CENTER, RIGHT, LEFT
};

class SILENCE_EXPORT Text2D
{
private:
    // The transfer object for the vertices
    GPU_Transfer * transfer;
    // The sample object for the texture
    GPU_Sampler * texture;
    // The height and width of the texture generated
    GLuint height;
    GLuint width;
public:
    // Constructor & Deconstructor
    Text2D();
    ~Text2D();

    // sets the font for text as well as the text to be rendered
    void setFont(FontAsset * text_font, const char * text);
    // sets the area the text should be rendered inside and its alignment
    void setArea(glm::vec2 size, ALIGNMENT textAlignment);
    // sets the area the text should be rendered inside
    void setArea(glm::vec2 size);

    std::string getText();
    // returns the ID for the texts texture
    GPU_ID getTextureID();
    // returns the ID for the vertex data
    GPU_ID getDataID();
private:
    std::string data;

};

#endif