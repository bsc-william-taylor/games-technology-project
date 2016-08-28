
#pragma once

#include "EngineLayer.h"
#include "Camera.h"

class SILENCE_EXPORT FirstPersonCamera : public Camera
{
    glm::mat4 viewMatrix;
    glm::vec4 cameraArea;
    glm::vec3 translate;
    glm::vec3 last;

    GLfloat terrainHeight;
    GLfloat velocity;
    GLfloat rotation;
    GLfloat pitch;
    GLfloat speed;
    GLfloat height;
    GLfloat axisV[2];
    GLint axis[2];
    GLint keys[4];

    bool cancel;
    bool enabled;
    bool gamepadEnabled;
    bool running;
public:
    explicit FirstPersonCamera(Camera *);
    FirstPersonCamera();
    ~FirstPersonCamera();

    glm::vec3 getPosition();
    glm::mat4& getView();

    void prepareCamera();
    void onGamepadAxis(int, float);
    void setCameraArea(glm::vec4);
    void setCameraDirection(float r, float p);
    void updateCameraPosition(SDL_Event&);
    void handleTerrainHeight(float);
    void setPosition(glm::vec3 p);
    void repositionCamera();
    void cancelMovement(glm::vec3);
    void cancelMovement();
    void walk();
    void run();

    float getPitch();
    float getHeight();
    float getSpeed();
    float getRotation();

    glm::vec3 getCameraDirection();

    GLvoid disable();
    GLvoid enable();
};