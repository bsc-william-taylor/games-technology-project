
#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera()
    : rotation(0.0), pitch(0.0), height(0.0F), speed(0.75)
{
    viewMatrix = glm::mat4(1.0);
    cameraArea = glm::vec4(-225, -135, 225, 135);

    translate = glm::vec3(0.0, -1000.0f, 0.0);
    last = glm::vec3(0.0, -1000.0f, 0.0);

    cancel = false;
    gamepadEnabled = false;
    enabled = true;

    for (auto i = 0; i < 4; i++) 
    {
        keys[i] = FALSE;
        if (i < 2) 
        {
            axis[i] = FALSE;
        }
    }

    terrainHeight = 0;
    velocity = 0.0f;
}

FirstPersonCamera::FirstPersonCamera(Camera * camera)
    : rotation(0.0), pitch(-50), height(0.0F), speed(0.75)
{
    cameraArea = glm::vec4(-225, -135, 225, 135);
    viewMatrix = camera->getView();
    translate = camera->getPosition();
    cancel = false;
    enabled = true;
    gamepadEnabled = false;

    for (int i = 0; i < 4; i++) {
        keys[i] = FALSE;
        if (i < 2) axis[i] = FALSE;
    }

    translate = glm::vec3(0.0, -1000.0f, 0.0);
    last = glm::vec3(0.0, -1000.0f, 0.0);
    terrainHeight = 0;
    velocity = 0.0f;
}

GLvoid FirstPersonCamera::disable()
{
    enabled = false;
}

GLvoid FirstPersonCamera::enable()
{
    enabled = true;
}

glm::vec3 FirstPersonCamera::getCameraDirection()
{
    double z = glm::cos(glm::radians(rotation));
    double y = glm::cos(glm::radians(pitch+90));
    double x = glm::sin(glm::radians(rotation));	

    return glm::normalize(glm::vec3(x, y, -z));
}

FirstPersonCamera::~FirstPersonCamera()
{
}

float FirstPersonCamera::getHeight()
{
    return height;
}

glm::mat4& FirstPersonCamera::getView()
{
    return viewMatrix;
}

void FirstPersonCamera::onGamepadAxis(int axisID, float angle)
{
    if (axisID == SDL_CONTROLLER_AXIS_LEFTX) {
        axis[0] = GL_TRUE;
        axisV[0] += angle * speed;
    }

    if (axisID == SDL_CONTROLLER_AXIS_LEFTY) {
        axis[1] = GL_TRUE;
        axisV[1] += angle * speed;
    }

    if (axisID == SDL_CONTROLLER_AXIS_RIGHTX) {
        rotation += angle * 1.75;
    }

    if (axisID == SDL_CONTROLLER_AXIS_RIGHTY) {
        float next = pitch + (angle * 1.75);
        if (next <= 55.0 && next >= -55.0) {
            pitch += angle * 1.75;
        }
    }
}

void FirstPersonCamera::walk()
{
    speed = 0.75;
}

void FirstPersonCamera::run()
{
    speed = 1.1;
}

void FirstPersonCamera::cancelMovement()
{
    cancel = true;
}

void FirstPersonCamera::handleTerrainHeight(float h)
{
    terrainHeight = h;
}

glm::vec3 FirstPersonCamera::getPosition()
{
    return -last;
}

void FirstPersonCamera::updateCameraPosition(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN) 
    {
        switch (e.key.keysym.sym) {
            case SDLK_w: keys[0] = TRUE;  break;
            case SDLK_s: keys[1] = TRUE;  break;
            case SDLK_a: keys[2] = TRUE;  break;
            case SDLK_d: keys[3] = TRUE;  break;
                default:
                    break;
        }
    }

    if (e.type == SDL_KEYUP) 
    {
        switch (e.key.keysym.sym) {
            case SDLK_w: keys[0] = FALSE;  break;
            case SDLK_s: keys[1] = FALSE;  break;
            case SDLK_a: keys[2] = FALSE;  break;
            case SDLK_d: keys[3] = FALSE;  break;
        default:
            break;
        }
    }

    if (e.type == SDL_MOUSEMOTION) 
    {
        rotation += e.motion.xrel;
        auto next = pitch + e.motion.yrel;
        
        if (next <= 55.0 && next >= -55.0) 
        {
            pitch += e.motion.yrel;
        }
        
        if (rotation < 0) 
        {
            rotation = 360;
        }

        if (rotation > 360) 
        {
            rotation = 0;
        }
    }
}

void FirstPersonCamera::setPosition(glm::vec3 position)
{
    for (auto i = 0; i < 4; i++) 
    {
        keys[i] = FALSE;
        if (i < 2) 
        {
            axis[i] = FALSE;
        }
    }

    translate = -position;
    translate.z = position.z;
    height = -position.y;
    terrainHeight = 0;
    cancel = false;
}

void FirstPersonCamera::prepareCamera()
{
    static auto f = 0.0f;
    static auto s = true;

    if (velocity > 0.1) 
    {
        if (speed >= 1.0) 
        {
            s = f >= 0.5f ? false : s;
            s = f <= -0.5f ? true : s;
            s ? f += 0.05 : -0.05;
        }
        else 
        {
            f = 0.0;
        }
    } 
    else 
    {
        f = 0.0;
    }
    
    viewMatrix = glm::mat4(1.0);
    viewMatrix = glm::rotate(viewMatrix, pitch, glm::vec3(1.0, 0.0, 0.0));
    viewMatrix = glm::rotate(viewMatrix, rotation, glm::vec3(0.0, 1.0, 0.0));
    viewMatrix = glm::rotate(viewMatrix, f, glm::vec3(0.0, 0.0, 1.0));

    last = glm::vec3(translate);
    last.y += ((terrainHeight + height) - last.y) / 10.0;

    if (keys[0]) 
    {
        last += glm::vec3(glm::sin(glm::radians(-rotation)), 0.0, glm::cos(glm::radians(-rotation))) * speed;
    } 
    
    if (keys[1]) 
    {
        last += glm::vec3(-glm::sin(glm::radians(-rotation)), 0.0, -glm::cos(glm::radians(-rotation)))  * speed;
    } 
    
    if (keys[2]) 
    {
        last += glm::vec3(glm::cos(glm::radians(-rotation)), 0.0, -glm::sin(glm::radians(-rotation)))  * speed;
    }

    if (keys[3]) 
    {
        last += glm::vec3(-glm::cos(glm::radians(-rotation)), 0.0, glm::sin(glm::radians(-rotation)))  * speed;
    }

    if (axis[0])
    {
        last += glm::vec3(glm::cos(glm::radians(-rotation)), 0.0, -glm::sin(glm::radians(-rotation)))  * -axisV[0];
        axisV[0] = 0.0;
        axis[0] = FALSE;
    }

    if (axis[1]) 
    {
        last += glm::vec3(glm::sin(glm::radians(-rotation)), 0.0, glm::cos(glm::radians(-rotation))) * -axisV[1];
        axisV[1] = 0.0;
        axis[1] = FALSE;
    }
}

void FirstPersonCamera::setCameraArea(glm::vec4 vec)
{
    cameraArea = vec;
}

void FirstPersonCamera::repositionCamera()
{
    velocity = glm::length(translate - last);
    
    if (last.x >= cameraArea.x && last.x <= cameraArea.z && 
        last.z >= cameraArea.y && last.z <= cameraArea.w && 
        !cancel && enabled)
    {
        translate = last;
    }

    viewMatrix = glm::translate(viewMatrix, translate);
    cancel = false;

    if (gamepadEnabled) 
    {
        for (auto i = 0; i < 4; i++) 
        {
            keys[i] = FALSE;
        }
    }
}

void FirstPersonCamera::setCameraDirection(float r, float p) {
    rotation = r;
    pitch = p;
}

float FirstPersonCamera::getPitch() 
{
    return pitch;
}

float FirstPersonCamera::getSpeed() 
{
    return velocity;
}

float FirstPersonCamera::getRotation()
{
    return rotation;
}