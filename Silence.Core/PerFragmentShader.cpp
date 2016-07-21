
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

#include "PerFragmentShader.h"

const char * fragSource = SRC(
    layout(location = 0) out vec4 FragmentColour;
    const int MAX_POINT_LIGHTS = 8;

    struct PointLight {
        vec3 pointPosition;
        vec3 pointColour;
        vec4 att; // constantA, ambientI, linearA, expA;			

        int pointActive;
    };

    struct SpotLight {
        vec3 spotColour;
        vec3 spotPosition;
        vec3 spotDirection;

        int active;

        float linearAttrib;
        float coneCosine;
    };

    struct DirectionalLight {
        vec3 lightDirection;
        vec3 lightColour;
    };

    uniform PointLight pointLights[MAX_POINT_LIGHTS];
    uniform DirectionalLight sceneLight;
    uniform SpotLight spotLight;

    uniform samplerCube cubeMap;
    uniform sampler2D sampler1;
    uniform sampler2D sampler2;
    uniform sampler2D sampler3;

    uniform float density = 0.0125;
    uniform float start = 1.0;
    uniform float end = 200.0;


    uniform int pointLightsCount;
    uniform int texture_count;
    uniform int fogEnabled;
    uniform float alpha;

    in vec4 materialSpacePosition;
    in vec4 materialPosition;
    in vec3 materialNormal;
    in vec3 uv_cords;

    vec4 getPointLight(const int pointID) {
        vec3 direction = materialPosition.xyz - pointLights[pointID].pointPosition.xyz;

        float diffuseAngle = max(0.0, dot(materialNormal.xyz, -normalize(direction)));

        float distanceToLight = length(direction.xyz);
        float fAttTotal = pointLights[pointID].att.x + pointLights[pointID].att.z *distanceToLight + pointLights[pointID].att.w*distanceToLight*distanceToLight;

        return vec4(pointLights[pointID].pointColour * (pointLights[pointID].att.y + diffuseAngle) / fAttTotal, 0.0);
    }

    vec4 getSpotLightColour(const SpotLight spotLight) {
        if (spotLight.active != 0) {
            float fDistance = distance(materialPosition.xyz, spotLight.spotPosition);
            
            vec3 vDir = normalize(materialPosition.xyz - spotLight.spotPosition);

            float fCosine = dot(spotLight.spotDirection, vDir);
            float fDif = 1.0 - spotLight.coneCosine;
            float fFactor = clamp((fCosine - spotLight.coneCosine) / fDif, 0.0, 1.0);
            
            if (fCosine > spotLight.coneCosine) {
                return vec4(spotLight.spotColour, 1.0) * fFactor / (fDistance * spotLight.linearAttrib);
            }
        }

        return vec4(0.0, 0.0, 0.0, 0.0);
    }

    vec4 getSunLight() {
        float sunLightI = max(0.0, dot(materialNormal, -normalize(sceneLight.lightDirection)));
        vec3 light = vec3(sceneLight.lightColour * sunLightI);
        return vec4(light, 1.0);
    }

    void main() {
        if (uv_cords.z == 0.0) {
            vec4 textureColour = vec4(0.0, 0.0, 0.0, 0.0);
            if (texture_count == 1) {
                textureColour = texture(sampler1, uv_cords.xy);
                if (textureColour.a == 0.0) {
                    discard;
                }
            } else {
                vec2 map = uv_cords.xy / vec2(128, 128);
                vec4 index = texture(sampler3, map.xy);

                float balance = 1.0 - index.r;

                textureColour += texture(sampler1, uv_cords.xy) * balance;
                textureColour += texture(sampler2, uv_cords.xy) * index.r;
            }

            vec4 spotLight = getSpotLightColour(spotLight);
            vec4 pointLight = vec4(0.0, 0.0, 0.0, 0.0);
            vec4 sunLight = getSunLight();
            
            for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
                if (pointLights[i].pointActive == 1) {
                    pointLight += getPointLight(i);
                }
            }

            FragmentColour = textureColour * (sunLight + spotLight + pointLight);
            FragmentColour.a = alpha;
        } else {
            FragmentColour = texture(cubeMap, uv_cords);
            FragmentColour.a = alpha;
        }

        if (fogEnabled == 1) {
            float fogCoord = abs(materialSpacePosition.z / materialSpacePosition.w);
            float fResult = exp(-density * fogCoord);
            fResult = 1.0 - clamp(fResult, 0.0, 1.0);

            FragmentColour = mix(FragmentColour, vec4(0.0, 0.0, 0.0, 1.0), fResult);
            FragmentColour.a = alpha;
        } else {
            FragmentColour.a = alpha;
        }

    }
);


// Constructor & Deconstructor
PerFragmentShader::PerFragmentShader()
    : program(NULL)
{
}

PerFragmentShader::~PerFragmentShader()
{
    SAFE_RELEASE(program)
}

void PerFragmentShader::compile()
{
    // make sure the program only compiles once
    if (!program)
    {
        // create the LLGI program object
        program = new GPU_Program();
        // then set the fragment source for the shader
        program->setFragmentSource(fragSource);

        // set the vertex source for the shader
        program->setVertexSource(SRC(
            layout(location = 0) in vec3 positions;
            layout(location = 1) in vec3 normals;
            layout(location = 2) in vec3 uvs;
            layout(location = 3) in vec3 indices;
            layout(location = 4) in mat4 models;

            uniform mat4 projection;
            uniform mat4 normal;
            uniform mat4 model;
            uniform mat4 view;

            uniform int sampler_index;
            uniform int instanceList;

            out vec4 materialSpacePosition;
            out vec4 materialPosition;
            out vec3 materialNormal;
            out vec3 uv_cords;

            void main() {
                if (instanceList == 0) {
                    gl_Position = projection * view * model * vec4(positions, 1.0);
                    materialSpacePosition = view * model * vec4(positions, 1.0);
                    materialPosition = model * vec4(positions, 1.0);
                } else{
                    gl_Position = projection * view * models * vec4(positions, 1.0);
                    materialSpacePosition = view * models * vec4(positions, 1.0);
                    materialPosition = models * vec4(positions, 1.0);
                }
                
                materialNormal = normalize((normal * vec4(normals, 0.0))).xyz;
                
                if (sampler_index == 0)	{
                    uv_cords = uvs;
                    uv_cords.z = 0.0;
                } else {
                    uv_cords = positions;
                }
            }
        ));

        // compiles the shader source and throws exception is it fails
        if (!program->compile())
        {
            throw Error(SYSTEM, "couldnt compile per fragment shader shader");
        }
    }
}

void PerFragmentShader::bind(std::string name, int ID, int value)
{
    glActiveTexture(GL_TEXTURE0+ID);
    glBindTexture(GL_TEXTURE_2D, value);
    glActiveTexture(GL_TEXTURE0);

    this->program->setInteger((GLchar *)name.c_str(), ID);
}

// runs the program and draws the vertexCount number of vertices
void PerFragmentShader::run(int vertexCount)
{
    program->run(vertexCount);
}

// prepares the object by binding the VAO
void PerFragmentShader::prepare(int ID)
{
    program->prepare(ID);
}

void PerFragmentShader::runIndex(int ID, int indicesSize)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

// runs the program with these specified geometry type and draws all vertices through i to x
void PerFragmentShader::run(int a, int i, int x)
{
    program->run(a, i, x);
}

// runs the program drawing the vertices though i to x
void PerFragmentShader::run(int i, int x)
{
    program->run(GL_TRIANGLES, i, x);
}

// binds the vao and binds a texture as well
void PerFragmentShader::prepare(int ID, int tID)
{
    program->prepare(ID);
    program->bind(tID);
}

// returns a pointer to the GPU_Program
GPU_Program * PerFragmentShader::getProgram()
{
    return program;
}