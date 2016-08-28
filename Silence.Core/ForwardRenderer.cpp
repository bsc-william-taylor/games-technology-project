
#include "ForwardRenderer.h"
#include "FirstPersonCamera.h"
#include "DefaultCamera.h"

ForwardRenderer::ForwardRenderer()
{
    sceneCamera = new DefaultCamera();
    cameraType = CameraType::DefaultCamera;
    fogEnabled = false;
}

ForwardRenderer::~ForwardRenderer()
{
    SAFE_RELEASE(sceneCamera);
}

void ForwardRenderer::onGamepadAxis(int i, float x)
{
    sceneCamera->onGamepadAxis(i, x);
}

void ForwardRenderer::createRenderer()
{
    shader.compile();
}

void ForwardRenderer::prepare()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto program = shader.getProgram();
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

void ForwardRenderer::present()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void ForwardRenderer::handleTerrainHeight(Heightmap * heightmap)
{
    auto heightmapSize = heightmap->getHeightmapSize();
    auto cameraPosition = -sceneCamera->getPosition();

    auto scale = heightmap->getScale();
    auto y = heightmap->getHeightAt(heightmapSize.x / 2 - (cameraPosition.x / scale), heightmapSize.y / 2 + (cameraPosition.z / scale));

    sceneCamera->handleTerrainHeight(y);
}

void ForwardRenderer::renderAnimatedModel(AnimatedModel * model)
{
    if (model != nullptr)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        shader.prepare(model->getVertexID(), model->getTextureID());
        shader.run(model->getVertDataCount());

        glDisable(GL_CULL_FACE);
    }
}

void ForwardRenderer::setAlpha(float alpha)
{
    shader.getProgram()->setFloat("alpha", alpha);
}

void ForwardRenderer::renderCube(Cube * cube)
{
    if (cube != nullptr)
    {
        shader.prepare(cube->getDataID(), cube->getTextureID());
        shader.run(CUBE);
    }
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null cube pointer", Author::William);
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
    if (heightmap != nullptr)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

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
        throw Error(Component::Renderer, "Error you tried to render a null map pointer", Author::William);
    }
}

void ForwardRenderer::updateCamera(SDL_Event& event)
{
    sceneCamera->updateCameraPosition(event);
}

void ForwardRenderer::repositionCamera()
{
    sceneCamera->repositionCamera();
}

void ForwardRenderer::setProjectionMatrix(GPU_Matrices& matrices)
{
    const auto program = shader.getProgram();
    program->setMatrix("projection", glm::value_ptr(matrices.getProjectionMatrix()));
}

void ForwardRenderer::setModelMatrix(GPU_Matrices& matrices)
{
    const auto program = shader.getProgram();
    program->setMatrix("normal", glm::value_ptr(matrices.getNormalMatrix()));
    program->setMatrix("model", glm::value_ptr(matrices.getModelMatrix()));
}

void ForwardRenderer::changeCamera(CameraType newCamera)
{
    switch (newCamera)
    {
        case CameraType::FirstPerson:
        {
            auto newSceneCamera = new FirstPersonCamera(sceneCamera);
            SAFE_RELEASE(sceneCamera);
            sceneCamera = newSceneCamera;
            cameraType = CameraType::FirstPerson;
            break;
        }

        default: break;
    }
}

void ForwardRenderer::setCameraPosition(glm::vec3 positions)
{
    sceneCamera->setPosition(positions);
}

CameraType ForwardRenderer::getCameraType()
{
    return cameraType;
}

void ForwardRenderer::renderModelSet(ModelSet * set)
{
    if (set != nullptr)
    {
        shader.getProgram()->setInteger("instanceList", 1);

        auto textures = set->getTextures();
        auto meshs = set->getMeshes()->getMeshes();

        glBindVertexArray(set->getMesh()->getID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, set->getMesh()->getIndexID());
        glActiveTexture(GL_TEXTURE0);

        for (GLuint i = 0; i < meshs.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, textures[meshs[i]->MaterialIndex]->getID());
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

void ForwardRenderer::renderModel(Model * model)
{
    if (model != nullptr)
    {
        auto textures = model->getTextures();
        auto meshs = model->getMeshes()->getMeshes();

        glBindVertexArray(model->getMesh()->getID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getMesh()->getIndexID());
        glActiveTexture(GL_TEXTURE0);

        for (GLuint i = 0; i < meshs.size(); i++)
        {	
            glBindTexture(GL_TEXTURE_2D, textures[meshs[i]->MaterialIndex]->getID());
            glDrawElementsBaseVertex(GL_TRIANGLES, meshs[i]->NumIndices, 
                GL_UNSIGNED_INT, meshs[i]->IndexStart, meshs[i]->VertexStart
            );
        }
    }
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null model pointer", Author::William);
    }
}

void ForwardRenderer::setCameraArea(glm::vec4 area)
{
    sceneCamera->setCameraArea(area);
}

void ForwardRenderer::renderSkybox(Skybox * skybox)
{
    if (skybox != nullptr)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, sceneCamera->getPosition());

        const auto program = shader.getProgram();
        program->setMatrix("model", glm::value_ptr(modelMatrix));
        program->setInteger("sampler_index", 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
        glUniform1i(glGetUniformLocation(program->getID(), "cubeMap"), 1);

        shader.prepare(skybox->getDataID());
        shader.run(CUBE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

        modelMatrix = glm::mat4(1.0);

        program->setInteger("sampler_index", 0);
        program->setInteger("texture_count", 1);
    }
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null skybox pointer", Author::William);
    }
}

Camera * ForwardRenderer::getCamera()
{
    return sceneCamera;
}

Lights * ForwardRenderer::getLights() 
{
    return &lights;
}