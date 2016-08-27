
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

#include "DeferredRenderer.h"
#include "FirstPersonCamera.h"

// Constructor & Deconstructor
DeferredRenderer::DeferredRenderer()
    : sphereAsset("no-name")
{
    // setup the default camera for the renderer
    sceneCamera = new DefaultCamera();
    // then setup some properties for the directional light
    lightPosition = glm::vec3(1.0, 0.0, 0.0);
    lightColour = glm::vec3(0.4, 0.4, 0.4);
    // the camera type
    cameraType = DEFAULT_CAMERA;
}

DeferredRenderer::~DeferredRenderer()
{
    // cleanup all allocated objects
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

// This function creates the renderer ready to draw objects
void DeferredRenderer::createRenderer()
{
    // first grab the sphere object from disk
    sphereAsset.grabFromFile("data/sphere.obj");

    // create a list of vertices for the quad which we need for the final image
    Vertices area = {
        Vertex(-1.0, -1.0, 0.0), Vertex(-1.0, 1.0, 0.0), Vertex(1.0, 1.0, 0.0),
        Vertex(-1.0, -1.0, 0.0), Vertex(1.0, -1.0, 0.0), Vertex(1.0, 1.0, 0.0)
    };

    // create the gpu transfer object for the quad 
    quad = new GPU_Transfer();
    quad->setVertices(area);
    quad->send();

    // and do the same for the sphere volume
    sphere = new Model();
    sphere->setModel(&sphereAsset);

    // then compile the shaders needed
    secondPassShader.compile();
    firstPassShader.compile();

    // and finally setup the geometry buffer that we need
    gbuffer = new GeometryBuffer();
    // and generate our UBO object
    glGenBuffers(1, &lID);
}

// returns a reference to the lights position in the scene
glm::vec3& DeferredRenderer::getLightPosition()
{
    return lightPosition;
}

// returns a reference to the lights colour in the scene
glm::vec3& DeferredRenderer::getLightColour()
{
    return lightColour;
}

// Prepares the renderer for rendering object
void DeferredRenderer::prepare()
{
    // indicate to the gbuffer we are about to start drawing geometry
    gbuffer->startFrame();
    gbuffer->prepareForWriting();

    // then get the first shaders program
    GPU_Program * program = firstPassShader.getProgram();

    // bind the shader and set the matrix and integer value
    program->bindShader();
    program->setMatrix("view", glm::value_ptr(sceneCamera->getView()));
    program->setInteger("sampler_index", 0);

    // set these uniform values
    glUniform1i(glGetUniformLocation(program->getID(), "sampler"), 0);
    glUniform1i(glGetUniformLocation(program->getID(), "cubeMap"), 1);

    // then disable blend for the first pass
    glDisable(GL_BLEND);
    // and enable depth testing and culling
    glEnable(GL_DEPTH_TEST);	
    // and also specify the types of each
    glDepthFunc(GL_LEQUAL);
}

/*
// the following copies all the lights in the vector into the renderers internal vector of lights
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

// the following code copies the final image produced into the framebuffer after a second pass
void DeferredRenderer::present()
{
    // first disable culling and writing into the depth buffer
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    // then enabled blending and the stencil test
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    // bind the second shader and set the screen size variable
    secondPassShader.getProgram()->bindShader();
    secondPassShader.getProgram()->setVector("screenSize", gbuffer->getBufferSize());

    // create the matrices needed for the program
    glm::mat4 projectionMatrix = glm::mat4();
    glm::mat4 modelMatrix = glm::mat4();
    glm::mat4 view = glm::mat4();

    // create the point light variable counter
    GLuint ID = 0;
    
    /*
    // then iterate through every light first rendering a volume then the light
//	for (auto& light : pointLights)
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

    // disable the stencil test and the depth test
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    // again prepare the buffer for reading
    gbuffer->prepareForReading();

    // set up a new projection matrix
    projectionMatrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // and perform the calculation for the directional light pass
    secondPassShader.getProgram()->setMatrix("projectionMatrix", glm::value_ptr(projectionMatrix));
    secondPassShader.getProgram()->setMatrix("model", glm::value_ptr(modelMatrix));
    secondPassShader.getProgram()->setMatrix("view", glm::value_ptr(view));
    secondPassShader.getProgram()->setInteger("lightPass", 0);
    secondPassShader.prepare(quad->getID());
    secondPassShader.run(QUAD);

    // then call this method which will copy the final image into the frame buffer
    gbuffer->stopFrame();

    // and disable the frame buffer and re enable writing to the depth buffer
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

// Handle the terrains height where we will place the camera on top of the height map
void DeferredRenderer::handleTerrainHeight(Heightmap * heightmap)
{
    // first we get the height maps size and the camera position
    glm::vec2 heightmapSize = heightmap->getHeightmapSize();
    glm::vec3 cameraPosition = sceneCamera->getPosition();

    // then we acquire the y value by calculating the point on the heightmap we are currently on
    float y = heightmap->getHeightAt(heightmapSize.x / 2 - cameraPosition.x, heightmapSize.y / 2 + cameraPosition.z);
    // we then send this new y value to the camera so it can reposition itself
    sceneCamera->handleTerrainHeight(y);
}

// renders animated model
void DeferredRenderer::renderAnimatedModel(AnimatedModel * model)
{
    if (model != NULL)
    {
        // then just render the object
        firstPassShader.prepare(model->getVertexID(), model->getTextureID());
        firstPassShader.run(model->getVertDataCount());
    }
}

// This function constructs the global UBO which holds all light information
void DeferredRenderer::constructUBO()
{	
    // We create our vector of vector data which will be sent to the ubo
    std::vector<float> ubo_data;

    // first we fill our directional light position into the ubo
    ubo_data.push_back(lightPosition.x);
    ubo_data.push_back(lightPosition.y);
    ubo_data.push_back(lightPosition.z);
    ubo_data.push_back(0.0f);	// needed for memory alignment reasons

    // then we fill our directional light colour into the ubo
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

    // then construct the actual UBO object to a global binding point so it can be shared accross multiple programs
    GLuint bindingPoint = 1, blockIndex;
    GLuint programID = secondPassShader.getProgram()->getID();

    blockIndex = glGetUniformBlockIndex(programID, "SceneLights");

    // we also add a hint that this data is dynamic, meaning it may change ona frequent basis
    glUniformBlockBinding(programID, blockIndex, bindingPoint);
    glBindBuffer(GL_UNIFORM_BUFFER, lID);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * ubo_data.size(), ubo_data.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, lID);
}

// This function simply renders a cube in 3D space
void DeferredRenderer::renderCube(Cube * cube)
{
    // make sure the pointer is value
    if (cube != NULL)
    {
        // then just render the object
        firstPassShader.prepare(cube->getDataID(), cube->getTextureID());
        firstPassShader.run(CUBE);
    } 
    else
    {
        // else throw an exception that indicates the error
        throw Error(Component::Renderer, "Error you tried to render a null cube pointer", Author::William);
    }
}

void DeferredRenderer::renderMap(Heightmap * heightmap)
{
    // make sure the pointer is value
    if (heightmap != NULL)
    {
        // then just render the object
        firstPassShader.prepare(heightmap->getVertexID(), heightmap->getTexture());
        firstPassShader.run(heightmap->getVertexCount());
    }
    else
    {
        // else throw an exception that indicates the error
        throw Error(Component::Renderer, "Error you tried to render a null map pointer", Author::William);
    }
}

// This function notify's the camera about a new system event
void DeferredRenderer::updateCamera(SDL_Event& event)
{
    sceneCamera->updateCameraPosition(event);
}

// This function tells the camera to calculate its new position
void DeferredRenderer::repositionCamera()
{
    sceneCamera->repositionCamera();
}

// This function sets the matrix value inside the shader used
void DeferredRenderer::setMatrixForObject(const char * name, glm::mat4& mat)
{
    GPU_Program * program = firstPassShader.getProgram();

    program->setMatrix((char *)name, glm::value_ptr(mat));
}

// This function changes the camera type via the CAMERA enum
void DeferredRenderer::changeCamera(CAMERA newCamera)
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
void DeferredRenderer::setCameraPosition(glm::vec3 positions)
{
    sceneCamera->setPosition(positions);
}

// returns teh type of camera being used
CAMERA DeferredRenderer::getCameraType()
{
    return cameraType;
}

// This function renders the model given
void DeferredRenderer::renderModel(Model * model)
{
    // prodiving the pointer is valid
    if (model != NULL)
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
        // else through an exception to say the pointer isnt valid
        throw Error(Component::Renderer, "Error you tried to render a null model pointer", Author::William);
    }
}

void DeferredRenderer::setCameraArea(glm::vec4 area)
{
    sceneCamera->setCameraArea(area);
}

// This function renders the skybox given
void DeferredRenderer::renderSkybox(Skybox * skybox)
{
    // prodiving the pointer is valid
    if (skybox != NULL)
    {
        // first create a new model matrix and set the translate it to the camera position
        glm::mat4 modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, -sceneCamera->getPosition());

        // then get the shader program
        GPU_Program * program = firstPassShader.getProgram();

        // and set the matrices values inside the shader as well as enable the cubemap
        program->setMatrix("model", glm::value_ptr(modelMatrix));
        program->setMatrix("view", glm::value_ptr(sceneCamera->getView()));
        program->setInteger("sampler_index", 1);

        // then disable culling and make the cubemap active
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
        glUniform1i(glGetUniformLocation(program->getID(), "cubeMap"), 1);

        // and render the cubemap which will be the skybox
        firstPassShader.prepare(skybox->getDataID());
        firstPassShader.run(CUBE);

        // then unbind the map and re enable culling
        glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);



        // and disable the map in the shader and resend an identity matrix to the shader
        modelMatrix = glm::mat4(1.0);

        program->setInteger("sampler_index", 0);
        program->setMatrix("model", glm::value_ptr(modelMatrix));
    }
    else
    {
        // else through an exception to say the pointer isnt valid
        throw Error(Component::Renderer, "Error you tried to render a null skybox pointer", Author::William);
    }
}