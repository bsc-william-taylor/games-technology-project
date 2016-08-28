
#pragma once

#include "Camera.h"

class SILENCE_EXPORT DefaultCamera : public Camera
{
    glm::mat4 viewMatrix;
public:
    DefaultCamera();
    ~DefaultCamera();
    
    glm::vec3 getPosition() override;
    glm::mat4& getView() override;

    void updateCameraPosition(SDL_Event&) override;
    void handleTerrainHeight(float f) override;
    void setPosition(glm::vec3) override;
    void repositionCamera() override;
    void cancelMovement() override;
    void prepareCamera() override;
    void onGamepadAxis(int, float) override;

    glm::vec3 getCameraDirection() override;

    GLfloat getHeight() override;
};