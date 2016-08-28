
#pragma once

#include "EngineLayer.h"
#include "math.h"

enum class CameraType { FirstPerson, DefaultCamera };

class SILENCE_EXPORT Camera 
{
public:
    virtual ~Camera() {}
    virtual void setCameraArea(glm::vec4){}
    virtual void setPosition(glm::vec3 p) = 0;
    virtual void setCameraDirection(float, float) {}
    virtual void updateCameraPosition(SDL_Event& w) = 0;
    virtual void handleTerrainHeight(GLfloat h) = 0;
    virtual void prepareCamera() = 0;
    virtual void repositionCamera() = 0;
    virtual void cancelMovement() = 0;
    virtual void onGamepadAxis(int, float) = 0;

    virtual float getRotation() { return 0.0; }
    virtual float getHeight() = 0;

    virtual glm::vec3 getCameraDirection() = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual glm::mat4& getView() = 0;
};