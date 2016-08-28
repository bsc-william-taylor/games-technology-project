
#include "DeferredRenderer.h"
#include "FirstPersonCamera.h"
#include "DefaultCamera.h"

DeferredRenderer::DeferredRenderer()
    : sphereAsset("no-name")
{
    sceneCamera = new DefaultCamera();
    lightPosition = vec3(1.0, 0.0, 0.0);
    lightColour = vec3(0.4, 0.4, 0.4);
    cameraType = CameraType::DefaultCamera;
}

DeferredRenderer::~DeferredRenderer()
{
    SAFE_RELEASE(sceneCamera);
    SAFE_RELEASE(gbuffer);
    SAFE_RELEASE(sphere);
    SAFE_RELEASE(quad);
}

void DeferredRenderer::onGamepadAxis(int i, float x)
{
    sceneCamera->onGamepadAxis(i, x);
}

void DeferredRenderer::setRenderRegion(int w, int h)
{
    gbuffer->setupGeometryBuffer(1920, 1080);
}

void DeferredRenderer::createRenderer()
{
    sphereAsset.grabFromFile("data/sphere.obj");

    Vertices area = {
        Vertex(-1.0, -1.0, 0.0), Vertex(-1.0, 1.0, 0.0), Vertex(1.0, 1.0, 0.0),
        Vertex(-1.0, -1.0, 0.0), Vertex(1.0, -1.0, 0.0), Vertex(1.0, 1.0, 0.0)
    };

    if(!quad)
    {
        quad = new GPU_Transfer();
        quad->setVertices(area);
        quad->send();
    }
    
    if(!sphere)
    {
        sphere = new Model();
        sphere->setModel(&sphereAsset);
    }
    
    secondPassShader.compile();
    firstPassShader.compile();
    
    if(!gbuffer)
    {
        gbuffer = new GeometryBuffer();
    }
    
    glGenBuffers(1, &lightID);
}

glm::vec3& DeferredRenderer::getLightPosition()
{
    return lightPosition;
}

glm::vec3& DeferredRenderer::getLightColour()
{
    return lightColour;
}

void DeferredRenderer::prepare()
{
    gbuffer->startFrame();
    gbuffer->prepareForWriting();

    const auto program = firstPassShader.getProgram();
    program->bindShader();
    program->setMatrix("view", glm::value_ptr(sceneCamera->getView()));
    program->setInteger("sampler_index", 0);

    glUniform1i(glGetUniformLocation(program->getID(), "sampler"), 0);
    glUniform1i(glGetUniformLocation(program->getID(), "cubeMap"), 1);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);	
    glDepthFunc(GL_LEQUAL);
}

/*
void DeferredRenderer::pushLights(std::vector<PointLight>& pointLights)
{
    // first we clear the vector
    this->pointLights.clear();

    // then add all the points lights specified
    for (auto& pointLight : pointLights)
    {
        this->pointLights.push_back(pointLight);
    }
}
*/

void DeferredRenderer::present()
{
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    // bind the second shader and set the screen size variable
    secondPassShader.getProgram()->bindShader();
    secondPassShader.getProgram()->setVector("screenSize", gbuffer->getBufferSize());

    // create the matrices needed for the program
    auto projectionMatrix = glm::mat4();
    auto modelMatrix = glm::mat4();
    auto view = glm::mat4();

    GLuint ID = 0;
    
    /*
    // then iterate through every light first rendering a volume then the light
	for (auto& light : pointLights)
    {
        // calculate the lights actual position in 3D space
        glm::vec3 lightPos = light.pointPosition / glm::vec3(light.area, light.area, light.area);

        // setup the projection matrix
        projectionMatrix = glm::perspective(60.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
        // and translate the light volume to its correct position
        modelMatrix = glm::scale(modelMatrix, glm::vec3(light.area, light.area, light.area));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.15, -1.0, 0.4));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(lightPos));

        // then make sure we are not drawing into any buffers apart from the stencil buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        // enable depth testing and set up stencil testing
        glEnable(GL_DEPTH_TEST);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

        secondPassShader.getProgram()->setMatrix("projectionMatrix", glm::value_ptr(projectionMatrix));
        secondPassShader.getProgram()->setMatrix("model", glm::value_ptr(modelMatrix));
        secondPassShader.getProgram()->setMatrix("view", glm::value_ptr(sceneCamera->getView()));

        renderVolume(ID, -1, modelMatrix, projectionMatrix);

        // the prepare the buffer for reading
        gbuffer->prepareForReading();

        // enable culling as we only want to render the lights effect on backward facing polygons
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        // make sure any fragment that isnt inside the volume is discards
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        // and disable depth testing
        glDisable(GL_DEPTH_TEST);

        // then render the lights effect inside the light volume
        renderVolume(ID, 1, modelMatrix, projectionMatrix);
        
        // disable culling
        glDisable(GL_CULL_FACE);
        // reset the model matrix
        modelMatrix = glm::mat4();
        // and increment the point light counter
        ++ID;
    }
    */

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    gbuffer->prepareForReading();

    projectionMatrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    secondPassShader.getProgram()->setMatrix("projectionMatrix", glm::value_ptr(projectionMatrix));
    secondPassShader.getProgram()->setMatrix("model", glm::value_ptr(modelMatrix));
    secondPassShader.getProgram()->setMatrix("view", glm::value_ptr(view));
    secondPassShader.getProgram()->setInteger("lightPass", 0);
    secondPassShader.prepare(quad->getID());
    secondPassShader.run(QUAD);

    gbuffer->stopFrame();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void DeferredRenderer::renderVolume(int ID, int LP, glm::mat4 m, glm::mat4 p)
{
    auto textures = sphere->getTextures();
    auto meshs = sphere->getMeshes();

    /*
    // and iterating through each mesh and drawing it
    for (GLuint i = 0; i < meshs.size(); i++)
    {
        secondPassShader.prepare(meshs[i]->getID());
        secondPassShader.getProgram()->setInteger("lightPass", LP);
        secondPassShader.getProgram()->setInteger("pointID", ID);
        secondPassShader.runIndex(meshs[i]->getIndexID(), meshs[i]->getIndexListSize());
    }
    */
}

void DeferredRenderer::handleTerrainHeight(Heightmap * heightmap)
{
    auto heightmapSize = heightmap->getHeightmapSize();
    auto cameraPosition = sceneCamera->getPosition();

    auto y = heightmap->getHeightAt(heightmapSize.x / 2 - cameraPosition.x, heightmapSize.y / 2 + cameraPosition.z);
    sceneCamera->handleTerrainHeight(y);
}

void DeferredRenderer::renderAnimatedModel(AnimatedModel * model)
{
    if (model != nullptr)
    {
        firstPassShader.prepare(model->getVertexID(), model->getTextureID());
        firstPassShader.run(model->getVertDataCount());
    }
}

void DeferredRenderer::constructUBO()
{	
    vector<float> ubo_data;
    ubo_data.push_back(lightPosition.x);
    ubo_data.push_back(lightPosition.y);
    ubo_data.push_back(lightPosition.z);
    ubo_data.push_back(0.0f);	

    ubo_data.push_back(lightColour.x);
    ubo_data.push_back(lightColour.y);
    ubo_data.push_back(lightColour.z);
    ubo_data.push_back(0.25F);
    
    /*
    // then we fill in each point light into the UBO
    for (auto& light : pointLights)
    {
        // first add the position
        ubo_data.push_back(light.pointPosition.x);
        ubo_data.push_back(light.pointPosition.y);
        ubo_data.push_back(light.pointPosition.z);
        ubo_data.push_back(0.0); // needed for memory alignment reasons

        // then add the colour
        ubo_data.push_back(light.pointColour.x);
        ubo_data.push_back(light.pointColour.y);
        ubo_data.push_back(light.pointColour.z);
        ubo_data.push_back(0.0); // needed for memory alignment reasons
        
        // finally add the attenuation values
        ubo_data.push_back(light.constantA);
        ubo_data.push_back(light.ambientI);
        ubo_data.push_back(light.linearA);
        ubo_data.push_back(light.expA);
    }
    */

    GLuint bindingPoint = 1, blockIndex;
    GLuint programID = secondPassShader.getProgram()->getID();

    blockIndex = glGetUniformBlockIndex(programID, "SceneLights");

    glUniformBlockBinding(programID, blockIndex, bindingPoint);
    glBindBuffer(GL_UNIFORM_BUFFER, lightID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * ubo_data.size(), ubo_data.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, lightID);
}

void DeferredRenderer::renderCube(Cube * cube)
{
    if (cube != nullptr)
    {
        firstPassShader.prepare(cube->getDataID(), cube->getTextureID());
        firstPassShader.run(CUBE);
    } 
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null cube pointer", Author::William);
    }
}

void DeferredRenderer::renderMap(Heightmap * heightmap)
{
    if (heightmap != nullptr)
    {
        firstPassShader.prepare(heightmap->getVertexID(), heightmap->getTexture());
        firstPassShader.run(heightmap->getVertexCount());
    }
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null map pointer", Author::William);
    }
}

void DeferredRenderer::updateCamera(SDL_Event& event)
{
    sceneCamera->updateCameraPosition(event);
}

void DeferredRenderer::repositionCamera()
{
    sceneCamera->repositionCamera();
}

void DeferredRenderer::setMatrixForObject(const char * name, glm::mat4& mat)
{
    const auto program = firstPassShader.getProgram();
    program->setMatrix(const_cast<char *>(name), glm::value_ptr(mat));
}

void DeferredRenderer::changeCamera(CameraType newCamera)
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

void DeferredRenderer::setCameraPosition(glm::vec3 positions)
{
    sceneCamera->setPosition(positions);
}

CameraType DeferredRenderer::getCameraType()
{
    return cameraType;
}

void DeferredRenderer::renderModel(Model * model)
{
    if (model != nullptr)
    {
        auto textures = model->getTextures();
        auto meshs = model->getMeshes();

        // and iterating through each mesh and drawing it
        //for (GLuint i = 0; i < meshs.size(); i++)
        {
            
            //firstPassShader.prepare(meshs[i]->getID(), textures[model->getMaterialID(i)]);
            //firstPassShader.runIndex(meshs[i]->getIndexID(), meshs[i]->getIndexListSize());
        }
    }
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null model pointer", Author::William);
    }
}

void DeferredRenderer::setCameraArea(glm::vec4 area)
{
    sceneCamera->setCameraArea(area);
}

void DeferredRenderer::renderSkybox(Skybox * skybox)
{
    if (skybox != nullptr)
    {
        auto modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, -sceneCamera->getPosition());

        const auto program = firstPassShader.getProgram();
        program->setMatrix("model", glm::value_ptr(modelMatrix));
        program->setMatrix("view", glm::value_ptr(sceneCamera->getView()));
        program->setInteger("sampler_index", 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
        glUniform1i(glGetUniformLocation(program->getID(), "cubeMap"), 1);

        firstPassShader.prepare(skybox->getDataID());
        firstPassShader.run(CUBE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

        modelMatrix = glm::mat4(1.0);

        program->setInteger("sampler_index", 0);
        program->setMatrix("model", glm::value_ptr(modelMatrix));
    }
    else
    {
        throw Error(Component::Renderer, "Error you tried to render a null skybox pointer", Author::William);
    }
}

Camera * DeferredRenderer::getCamera() 
{
    return sceneCamera;
}