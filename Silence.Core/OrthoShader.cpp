
#include "OrthoShader.h"

OrthoShader::OrthoShader()
    : program(nullptr)
{
}

OrthoShader::~OrthoShader()
{
    SAFE_RELEASE(program)
}

void OrthoShader::compile()
{
    if (program == nullptr)
    {
        program = new GPU_Program();
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

        if (!program->compile())
        {
            // throw an exception
            throw Error(Component::Renderer, "Couldnt compile ortho shader");
        }
    }
}

void OrthoShader::run(int vertexCount)
{
    program->run(vertexCount);
}

void OrthoShader::prepare(int ID, int tID)
{
    program->prepare(ID);
    program->bind(tID);
}

GPU_Program * OrthoShader::getProgram()
{
    return program;
}