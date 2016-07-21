
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

#pragma once

#include "EngineLayer.h"
#include "Camera.h"

class SILENCE_EXPORT FirstPersonCamera : public Camera
{
private:
    // The view matrix for the camera
    glm::mat4 viewMatrix;
    // The area the camera can walk around in
    glm::vec4 cameraArea;
    // The current movement value
    glm::vec3 translate;
    //
    glm::vec3 last;
    // the rotation of the camera
    GLfloat rotation;
    // the pitch of the camera
    GLfloat pitch;
    // the camera speed of movement
    GLfloat speed;

    GLfloat terrainHeight; 
    // the height of the camera
    GLfloat height;
    //
    GLint axis[2];
    //
    GLfloat axisV[2];
    // key state variables
    GLint keys[4];
    // should we cancel movement
    bool cancel;

    bool enabled;
public:
    // Constructors and Deconstructor
    FirstPersonCamera(Camera *);
    FirstPersonCamera();
    ~FirstPersonCamera();

    // returns a copy of the current position
    glm::vec3 getPosition();
    // returns the view matrix as a reference
    glm::mat4& getView();

    // this function will updates the camera based on the key being pressed
    void updateCameraPosition(SDL_Event&);
    // this function will reposition the camera to a certain height
    void handleTerrainHeight(float);
    // this function will set the position of the camera
    void setPosition(glm::vec3 p);
    // this function will reposition the camera by updating the view matrix
    void repositionCamera();

    void cancelMovement(glm::vec3);
    //
    void cancelMovement();
    //
    void walk();
    void run();

    float getSpeed() {
        return velocity;
    }

    //
    float getRotation(){ return rotation; }
    //
    void prepareCamera();
    //
    void onGamepadAxis(int, float);
    //
    void setCameraArea(glm::vec4);

    void setCameraDirection(float r, float p) {
        rotation = r;
        pitch = p;
    }

    float getPitch() {
        return pitch;
    }

    glm::vec3 getCameraDirection();

    // finally this function returns the height of the camera in 3D space
    GLfloat getHeight();

    GLvoid disable();
    GLvoid enable();

    bool gamepadEnabled;
    bool running;

    float velocity;
};