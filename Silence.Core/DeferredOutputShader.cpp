
#include "DeferredOutputShader.h"

DeferredOutputShader::DeferredOutputShader()
    : program(nullptr)
{
}

DeferredOutputShader::~DeferredOutputShader()
{
    SAFE_RELEASE(program)
}

void DeferredOutputShader::compile()
{
    if (program == nullptr)
    {
        program = new GPU_Program();
        program->setVertexSource(SRC(
            layout(location = 0) in vec3 positions;
            layout(location = 1) in vec3 normals;
            layout(location = 2) in vec3 uvs;

            uniform mat4 projectionMatrix;
            uniform mat4 model;
            uniform mat4 view;

            void main() {
                gl_Position = projectionMatrix * view * model * vec4(positions, 1.0);
            }
        ));

        program->setFragmentSource(SRC(
            layout(location = 0) out vec4 fragment_colour;

            const int NULL_PASS = - 1;
            const int POINT_PASS = 1;
            const int SUN_PASS = 0;

            struct PointLight {
                vec4 pointPosition;
                vec4 pointColour; 
                vec4 att; // constantA, ambientI, linearA, expA;			
            };

            struct DirectionalLight {
                vec4 lightDirection;
                vec4 lightColour;
            };

            layout(std140) uniform SceneLights {
                DirectionalLight sceneLight;
                PointLight pointLights[50];
            };
            
            uniform sampler2D PositionTexture;
            uniform sampler2D NormalTexture;
            uniform sampler2D ColourTexture;
            uniform vec2 screenSize;
            uniform int lightPass;
            uniform int pointID;
        
            vec4 materialPosition;
            vec4 materialColour;
            vec4 materialNormal;

            vec4 getPointLight()
            {
                vec3 direction = materialPosition.xyz - pointLights[pointID].pointPosition.xyz;
    
                float diffuseAngle = max(0.0, dot(materialNormal.xyz, -normalize(direction)));

                float distanceToLight = length(direction);
                float fAttTotal = pointLights[pointID].att.x + pointLights[pointID].att.z *distanceToLight + pointLights[pointID].att.w*distanceToLight*distanceToLight;

                return vec4(pointLights[pointID].pointColour * (pointLights[pointID].att.y + diffuseAngle) / fAttTotal);
            }

            vec4 getSunLight()
            {
                float sunLightI = max(0.0, dot(materialNormal.xyz, -normalize(sceneLight.lightDirection.xyz)));

                return vec4((sceneLight.lightColour * (sunLightI + sceneLight.lightColour.w)));
            }
    
            void main()
            {
                if (lightPass != NULL_PASS)
                {
                    vec2 uv_s = gl_FragCoord.xy / screenSize;

                    materialPosition = texture(PositionTexture, uv_s);
                    materialColour = texture(ColourTexture, uv_s);
                    materialNormal = texture(NormalTexture, uv_s);

                    if (materialNormal.xyz != vec3(0.0, 0.0, 0.0))
                    {
                        if (lightPass == SUN_PASS)
                        {
                            fragment_colour = (materialColour *  getSunLight());
                        }

                        if (lightPass == POINT_PASS)
                        {
                            fragment_colour = (materialColour * getPointLight());
                        }
                    }
                    else
                    {
                        fragment_colour = materialColour;
                    }
                }
                else
                {
                    fragment_colour = vec4(0.5, 0.5, 0.5, 1.0);
                }
                
            }
        ));

        if (!program->compile())
        {
            throw Error(Component::System, "couldnt compile deferred output shader");
        }
    }
}

void DeferredOutputShader::runIndex(int ID, int indicesSize)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, nullptr);
}
 
void DeferredOutputShader::run(int vertexCount)
{
    program->run(vertexCount);
}

void DeferredOutputShader::run(int i, int x)
{
    program->run(i, x);
}

void DeferredOutputShader::prepare(int ID)
{
    program->prepare(ID);

    GLuint programID = program->getID(); 
    GLuint positionT = glGetUniformLocation(programID, "PositionTexture");
    GLuint normalT = glGetUniformLocation(programID, "NormalTexture");
    GLuint colourT = glGetUniformLocation(programID, "ColourTexture");
    
    glUniform1i(positionT, 0);
    glUniform1i(normalT, 1);
    glUniform1i(colourT, 2);
}

GPU_Program * DeferredOutputShader::getProgram()
{
    return program;
}