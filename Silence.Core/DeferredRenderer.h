
#pragma once

#include "DeferredOutputShader.h"
#include "DeferredInputShader.h"
#include "FirstPersonCamera.h"
#include "GeometryBuffer.h"
#include "DefaultCamera.h"
#include "Heightmap.h"
#include "md2Model.h"
#include "Skybox.h"
#include "Model.h"
#include "Cube.h"
#include "GPU.h"

class SILENCE_EXPORT DeferredRenderer
{
    DeferredOutputShader secondPassShader;
    DeferredInputShader firstPassShader;
    GeometryBuffer * gbuffer;
    Camera * sceneCamera;
    CameraType cameraType;
public:
    DeferredRenderer();
    ~DeferredRenderer();

    void setRenderRegion(int, int);
    void setMatrixForObject(const char *, glm::mat4&);
    void handleTerrainHeight(Heightmap * heightmap);
    void setCameraPosition(glm::vec3 positions);
    void changeCamera(CameraType newCamera);
    void createRenderer();
    void updateCamera(SDL_Event& event);
    void repositionCamera();
    void onGamepadAxis(int i, float x);

    void renderSkybox(Skybox * skybox);
    void renderAnimatedModel(AnimatedModel * model);
    void renderMap(Heightmap * map);
    void renderModel(Model * model);
    void renderCube(Cube *  cube);
    void setCameraArea(glm::vec4);
    void constructUBO();
    void prepare();
    void present();

    CameraType getCameraType();

    Camera * getCamera() {
        return sceneCamera;
    }

    // returns a reference to the directional lights position
    glm::vec3& getLightPosition();
    // returns a reference to the directional lights colour
    glm::vec3& getLightColour();
private:
    //
    void renderVolume(int, int, glm::mat4, glm::mat4);
    // The transfer object for the sphere (used for light volumes)
    Model * sphere;
    // The transfer object for the quad (used for the final image)
    GPU_Transfer * quad;
    // our UBO light ID
    GpuID lID;
private:
    // This is our vector of point lights for the scene
    //std::vector<PointLight> pointLights;
    // this is the sphere volume used for light columns
    ModelAsset sphereAsset;
    
    // final these 2 variables are for the directional light
    glm::vec3 lightPosition;
    glm::vec3 lightColour;
};