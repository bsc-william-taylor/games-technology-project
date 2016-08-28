#include "DeferredInputShader.h"

DeferredInputShader::DeferredInputShader()
    : program(nullptr)
{
}

DeferredInputShader::~DeferredInputShader()
{
    SAFE_RELEASE(program)
}

void DeferredInputShader::compile()
{
    if (!program)
    {
        program = new GPU_Program();
        program->setFragmentSource(SRC(
            layout(location = 0) out vec3 PositionData;
            layout(location = 1) out vec3 NormalData;
            layout(location = 2) out vec3 ColorData;

            uniform sampler2D sampler;
            uniform samplerCube cubeMap;
            uniform int sampler_index;

            in vec3 position;
            in vec3 normal;
            in vec3 uv_cords;

            void main() 
            {
                if (sampler_index == 0)
                {
                    ColorData = texture(sampler, uv_cords.xy).rgb;
                }
                else
                {
                    ColorData = texture(cubeMap, uv_cords).rgb;
                }

                PositionData = position;
                NormalData = normal;
            }
        ));

        program->setVertexSource(SRC(
            layout(location = 0) in vec3 positions;
            layout(location = 1) in vec3 normals;
            layout(location = 2) in vec3 uvs;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;
            uniform int sampler_index;

            out vec3 position;
            out vec3 normal;
            out vec3 uv_cords;

            void main() {
                gl_Position = projection * view * model * vec4(positions, 1.0);

                position = (model * vec4(positions, 1.0)).xyz;
                normal = normalize((vec4(normals, 1.0))).xyz;

                if (sampler_index == 0)
                {
                    uv_cords = uvs;
                }
                else
                {
                    uv_cords = positions;
                }
                
            }
        ));

        if (!program->compile())
        {
            throw Error(Component::System, "couldnt compile deferred input shader");
        }
    }
}

void DeferredInputShader::run(int vertexCount)
{
    program->run(vertexCount);
}

void DeferredInputShader::prepare(int ID)
{
    program->prepare(ID);
}

void DeferredInputShader::runIndex(int ID, int indicesSize)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, nullptr);
}

void DeferredInputShader::run(int a, int i, int x)
{
    program->run(a, i, x);
}

void DeferredInputShader::run(int i, int x)
{
    program->run(GL_TRIANGLES, i, x);
}

void DeferredInputShader::prepare(int ID, int tID)
{
    program->prepare(ID);
    program->bind(tID);
}

GPU_Program * DeferredInputShader::getProgram()
{
    return program;
}