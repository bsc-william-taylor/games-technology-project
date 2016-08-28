#pragma once

#include "PerFragmentShader.h"
#include "FirstPersonCamera.h"
#include "GeometryBuffer.h"
#include "DefaultCamera.h"
#include "Heightmap.h"
#include "ModelSet.h"
#include "md2Model.h"
#include "Skybox.h"
#include "Lights.h"
#include "Model.h"
#include "Cube.h"
#include "GPU.h"

class SILENCE_EXPORT ForwardRenderer
{
    PerFragmentShader shader;
    Camera * sceneCamera;
    bool fogEnabled;
    CameraType cameraType;
    Lights lights;
public:
    ForwardRenderer();
    ~ForwardRenderer();

    Lights * getLights() {
        return &lights;
    }

    void setCameraDirection(float, float);
    
    void enableFog();
    void disableFog();

    //
    void disableWireframeMode();
    //
    void enableWireframeMode();
    //
    void setProjectionMatrix(GPU_Matrices& matrices);
    void setModelMatrix(GPU_Matrices& matrices);
    void handleTerrainHeight(Heightmap * heightmap);
    void setCameraPosition(glm::vec3 positions);
    void changeCamera(CameraType newCamera);
    void createRenderer();
    void updateCamera(SDL_Event& event);
    // this function updates the view matrix for the scene
    void repositionCamera();
    //
    void setAlpha(float);
    //
    void onGamepadAxis(int i, float x);
public:
    // this function renders a skybox in 3D space
    void renderSkybox(Skybox * skybox);
    // renders animated model
    void renderAnimatedModel(AnimatedModel * model);
    //
    void renderModelSet(ModelSet * set);
    // this function renders a heightmap in 3D space
    void renderMap(Heightmap * map);
    // this function renders a model in 3D space
    void renderModel(Model * model);
    // this function renders a cube in 3D space
    void renderCube(Cube *  cube);
    //
    void setCameraArea(glm::vec4);
    // this function prepares the renderer for rendering objects
    void prepare();
    // this function presents the final image
    void present();

    // returns the type of camera being used in the scene
    CameraType getCameraType();

    Camera * getCamera() {
        return sceneCamera;
    }
};