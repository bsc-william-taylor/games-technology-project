
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

#include "OrthoShader.h"

// Constructor & Deconstructor
OrthoShader::OrthoShader()
    : program(NULL)
{
}

OrthoShader::~OrthoShader()
{
    SAFE_RELEASE(program)
}

void OrthoShader::compile()
{
    // we should only compile once so if we haven't compiled before
    if (program == NULL)
    {
        // Create the program object
        program = new GPU_Program();
        // set the source code for the fragment shader
        program->setFragmentSource(SRC(
            layout(location = 0) out vec4 gl_FragColor;
            uniform sampler2D sampler;

            in float globalAlpha;
            in vec2 uv_cords;

            void main() {
                gl_FragColor = texture2D(sampler, uv_cords);
                gl_FragColor.a *= globalAlpha;
            }
        ));

        // set the source code for the vertex shader
        program->setVertexSource(SRC(
            layout(location = 0) in vec3 positions;
            layout(location = 1) in vec3 normals;
            layout(location = 2) in vec3 uvs;
            layout(location = 3) in vec3 colour;

            uniform mat4 projection;
            uniform mat4 model;
            uniform float alpha;

            out float globalAlpha;
            out vec2 uv_cords;

            void main() {
                vec4 finalPosition = projection * model * vec4(positions, 1.0);
                gl_Position = vec4(finalPosition.xy, 0.0, 1.0);
                globalAlpha = alpha;
                uv_cords = uvs.xy;
            }
        ));

        // Then compile the shader ready for use if we fail
        if (!program->compile())
        {
            // throw an exception
            throw Error(RENDERER, "couldnt compile ortho shader");
        }
    }
}

// runs the program and draw vertexCount number of vertices
void OrthoShader::run(int vertexCount)
{
    // just call the progarms run method
    program->run(vertexCount);
}

// This function prepares the program for use by
void OrthoShader::prepare(int ID, int tID)
{
    // bind an mesh ID / VAO
    program->prepare(ID);
    // and the texture it will use
    program->bind(tID);
}

// just returns a point to the program
GPU_Program * OrthoShader::getProgram()
{
    return program;
}