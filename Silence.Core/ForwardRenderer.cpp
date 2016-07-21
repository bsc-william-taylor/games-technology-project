
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

#include "ForwardRenderer.h"
#include "FirstPersonCamera.h"

// Constructor & Deconstructor
ForwardRenderer::ForwardRenderer()
{
    // setup the default camera for the renderer
    sceneCamera = new DefaultCamera();
    // the camera type
    cameraType = DEFAULT_CAMERA;
    fogEnabled = false;
}

ForwardRenderer::~ForwardRenderer()
{
    // cleanup all allocated objects
    SAFE_RELEASE(sceneCamera);
}

void ForwardRenderer::onGamepadAxis(int i, float x)
{
    sceneCamera->onGamepadAxis(i, x);
}

// This function creates the renderer ready to draw objects
void ForwardRenderer::createRenderer()
{
    // then compile the shaders needed
    shader.compile();
}

// Prepares the renderer for rendering object
void ForwardRenderer::prepare()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // then get the first shaders program
    GPU_Program * program = shader.getProgram();

    // bind the shader and set the matrix and integer value
    program->bindShader();
    program->setMatrix("view", glm::value_ptr(sceneCamera->getView()));
    program->setInteger("sampler_index", 0);
    program->setInteger("texture_count", 1);
    program->setInteger("instanceList", 0);
    program->setInteger("sampler1", 0);
    program->setInteger("cubeMap", 1);

    lights.sendLights(sceneCamera->getPosition(), sceneCamera->getCameraDirection(), program);

    if (fogEnabled)
    {
        program->setInteger("fogEnabled", 1);
    }
    else
    {
        program->setInteger("fogEnabled", 0);
    }
}

void ForwardRenderer::enableFog()
{
    fogEnabled = true;
}

void ForwardRenderer::disableFog()
{
    fogEnabled = false;
}

void ForwardRenderer::enableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void ForwardRenderer::disableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// the following code copies the final image produced into the framebuffer after a second pass
void ForwardRenderer::present()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

// Handle the terrains height where we will place the camera on top of the height map
void ForwardRenderer::handleTerrainHeight(Heightmap * heightmap)
{
    // first we get the height maps size and the camera position
    glm::vec2 heightmapSize = heightmap->getHeightmapSize();
    glm::vec3 cameraPosition = -sceneCamera->getPosition();

    float scale = heightmap->getScale();

    // then we acquire the y value by calculating the point on the heightmap we are currently on
    float y = heightmap->getHeightAt(heightmapSize.x / 2 - (cameraPosition.x / scale), heightmapSize.y / 2 + (cameraPosition.z / scale));
    // we then send this new y value to the camera so it can reposition itself
    sceneCamera->handleTerrainHeight(y);
}

// renders animated model
void ForwardRenderer::renderAnimatedModel(AnimatedModel * model)
{
    if (model != NULL)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        // then just render the object
        shader.prepare(model->getVertexID(), model->getTextureID());
        shader.run(model->getVertDataCount());

        glDisable(GL_CULL_FACE);
    }
}


void ForwardRenderer::setAlpha(float alpha)
{
    shader.getProgram()->setFloat("alpha", alpha);
}

// This function simply renders a cube in 3D space
void ForwardRenderer::renderCube(Cube * cube)
{
    // make sure the pointer is value
    if (cube != NULL)
    {
        // then just render the object
        shader.prepare(cube->getDataID(), cube->getTextureID());
        shader.run(CUBE);
    }
    else
    {
        // else throw an exception that indicates the error
        throw Error(RENDERER, "Error you tried to render a null cube pointer", William);
    }
}


void ForwardRenderer::setCameraDirection(float angle, float pitch)
{
    if (sceneCamera != NULL)
    {
        sceneCamera->setCameraDirection(angle, pitch);
    }
}
void ForwardRenderer::renderMap(Heightmap * heightmap)
{
    // make sure the pointer is value
    if (heightmap != NULL)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // then just render the object
        shader.prepare(heightmap->getVertexID(), heightmap->getTexture());

        if (heightmap->hasOverlay())
        {
            shader.getProgram()->setInteger("texture_count", 2);
            shader.bind("sampler2", 2, heightmap->getOverlayID());
            shader.bind("sampler3", 3, heightmap->getPathID());
        }

        shader.run(heightmap->getVertexCount());

        if (heightmap->hasOverlay())
        {
            shader.getProgram()->setInteger("texture_count", 1);
        }

        glDisable(GL_CULL_FACE);
        
    }
    else
    {
        // else throw an exception that indicates the error
        throw Error(RENDERER, "Error you tried to render a null map pointer", William);
    }
}

// This function notify's the camera about a new system event
void ForwardRenderer::updateCamera(SDL_Event& event)
{
    sceneCamera->updateCameraPosition(event);
}

// This function tells the camera to calculate its new position
void ForwardRenderer::repositionCamera()
{
    sceneCamera->repositionCamera();
}

void ForwardRenderer::setProjectionMatrix(GPU_Matrices& matrices)
{
    GPU_Program * program = shader.getProgram();

    program->setMatrix("projection", glm::value_ptr(matrices.getProjectionMatrix()));
}

void ForwardRenderer::setModelMatrix(GPU_Matrices& matrices)
{
    GPU_Program * program = shader.getProgram();

    program->setMatrix("normal", glm::value_ptr(matrices.getNormalMatrix()));
    program->setMatrix("model", glm::value_ptr(matrices.getModelMatrix()));
}


// This function changes the camera type via the CAMERA enum
void ForwardRenderer::changeCamera(CAMERA newCamera)
{
    // figure out which camera to change to
    switch (newCamera)
    {
        // if a first person camera
        case FIRST_PERSON:
        {
            // allocate the new camera with a copy constructor
            auto newSceneCamera = new FirstPersonCamera(sceneCamera);
            SAFE_RELEASE(sceneCamera);
            sceneCamera = newSceneCamera;
            // and change the type variable
            cameraType = FIRST_PERSON;
            break;
        }

            // else break as no other camera is supported yet
        default: break;
    }
}

// Set the camera position
void ForwardRenderer::setCameraPosition(glm::vec3 positions)
{
    sceneCamera->setPosition(positions);
}

// returns teh type of camera being used
CAMERA ForwardRenderer::getCameraType()
{
    return cameraType;
}

void ForwardRenderer::renderModelSet(ModelSet * set)
{
    if (set != NULL)
    {
        shader.getProgram()->setInteger("instanceList", 1);

        auto textures = set->getTextures();
        auto meshs = set->getMeshes()->getMeshes();

        glBindVertexArray(set->getMesh()->getID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, set->getMesh()->getIndexID());
        glActiveTexture(GL_TEXTURE0);

        for (GLuint i = 0; i < meshs.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, textures[meshs[i]->MaterialIndex]);
            glDrawElementsInstancedBaseVertex(GL_TRIANGLES, 
                meshs[i]->NumIndices,
                GL_UNSIGNED_INT, 
                meshs[i]->IndexStart,  
                set->getSetSize(),
                meshs[i]->VertexStart
            );
        }

        shader.getProgram()->setInteger("instanceList", 0);
    }
}

// This function renders the model given
void ForwardRenderer::renderModel(Model * model)
{
    // prodiving the pointer is valid
    if (model != NULL)
    {
        auto textures = model->getTextures();
        auto meshs = model->getMeshes()->getMeshes();

        glBindVertexArray(model->getMesh()->getID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getMesh()->getIndexID());
        glActiveTexture(GL_TEXTURE0);

        for (GLuint i = 0; i < meshs.size(); i++)
        {	
            glBindTexture(GL_TEXTURE_2D, textures[meshs[i]->MaterialIndex]);
            glDrawElementsBaseVertex(GL_TRIANGLES, meshs[i]->NumIndices, 
                GL_UNSIGNED_INT, meshs[i]->IndexStart, meshs[i]->VertexStart
            );
        }
    }
    else
    {
        // else through an exception to say the pointer isnt valid
        throw Error(RENDERER, "Error you tried to render a null model pointer", William);
    }
}

void ForwardRenderer::setCameraArea(glm::vec4 area)
{
    sceneCamera->setCameraArea(area);
}

// This function renders the skybox given
void ForwardRenderer::renderSkybox(Skybox * skybox)
{
    // prodiving the pointer is valid
    if (skybox != NULL)
    {
        // first create a new model matrix and set the translate it to the camera position
        glm::mat4 modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, sceneCamera->getPosition());

        // then get the shader program
        GPU_Program * program = shader.getProgram();

        // and set the matrices values inside the shader as well as enable the cubemap
        program->setMatrix("model", glm::value_ptr(modelMatrix));
        program->setInteger("sampler_index", 1);

        // then disable culling and make the cubemap active
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
        glUniform1i(glGetUniformLocation(program->getID(), "cubeMap"), 1);

        // and render the cubemap which will be the skybox
        shader.prepare(skybox->getDataID());
        shader.run(CUBE);

        // then unbind the map and re enable culling
        glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

        // and disable the map in the shader and resend an identity matrix to the shader
        modelMatrix = glm::mat4(1.0);

        program->setInteger("sampler_index", 0);
        program->setInteger("texture_count", 1);
    }
    else
    {
        // else through an exception to say the pointer isnt valid
        throw Error(RENDERER, "Error you tried to render a null skybox pointer", William);
    }
}