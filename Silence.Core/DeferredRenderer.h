
#pragma once

#include "DeferredOutputShader.h"
#include "DeferredInputShader.h"
#include "FirstPersonCamera.h"
#include "GeometryBuffer.h"
#include "Heightmap.h"
#include "AnimatedModel.h"
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
    Model * sphere;
    GPU_Transfer * quad;
    GpuID lightID;
    ModelAsset sphereAsset;

    vec3 lightPosition;
    vec3 lightColour;
public:
    DeferredRenderer();
    ~DeferredRenderer();

    void setRenderRegion(int, int);
    void setMatrixForObject(const char *, glm::mat4&);
    void setCameraPosition(glm::vec3 positions);
    void setCameraArea(glm::vec4);

    void onGamepadAxis(int i, float x);
    void handleTerrainHeight(Heightmap * heightmap);
    void updateCamera(SDL_Event& event);
    void changeCamera(CameraType newCamera);
    void repositionCamera();
    void createRenderer();

    void renderSkybox(Skybox * skybox);
    void renderAnimatedModel(AnimatedModel * model);
    void renderMap(Heightmap * map);
    void renderModel(Model * model);
    void renderCube(Cube *  cube);
    void constructUBO();
    void prepare();
    void present();

    CameraType getCameraType();
    Camera * getCamera();

    vec3& getLightPosition();
    vec3& getLightColour();
private:
    void renderVolume(int, int, glm::mat4, glm::mat4);
};