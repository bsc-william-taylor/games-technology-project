#pragma once

#include "PerFragmentShader.h"
#include "FirstPersonCamera.h"
#include "Heightmap.h"
#include "ModelSet.h"
#include "AnimatedModel.h"
#include "Skybox.h"
#include "Lights.h"
#include "Model.h"
#include "Cube.h"
#include "GPU.h"

class SILENCE_EXPORT ForwardRenderer
{
    PerFragmentShader shader;
    Lights lights;
    bool fogEnabled;
    CameraType cameraType;
    Camera * sceneCamera;
public:
    ForwardRenderer();
    ~ForwardRenderer();

    Lights * getLights();

    void setCameraDirection(float, float);
    void setCameraPosition(glm::vec3 positions);
    void setProjectionMatrix(GPU_Matrices& matrices);
    void setModelMatrix(GPU_Matrices& matrices);
    void setAlpha(float);
    void setCameraArea(glm::vec4);

    void enableWireframeMode();
    void enableFog();
    void disableWireframeMode();
    void disableFog();

    void onGamepadAxis(int i, float x);
    void handleTerrainHeight(Heightmap * heightmap);
    void changeCamera(CameraType newCamera);
    void updateCamera(SDL_Event& event);
    void repositionCamera();
    void createRenderer();
    void prepare();
    void present();

    void renderSkybox(Skybox * skybox);
    void renderAnimatedModel(AnimatedModel * model);
    void renderModelSet(ModelSet * set);
    void renderMap(Heightmap * map);
    void renderModel(Model * model);
    void renderCube(Cube *  cube);
   
    CameraType getCameraType();
    Camera * getCamera();
};