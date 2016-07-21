
#include "GPU_Matrices.h"

GPU_Matrices::GPU_Matrices()
{
    projectionMatrix.push(glm::mat4(1.0));
    normalMatrix.push(glm::mat4(1.0));
    modelMatrix.push(glm::mat4(1.0));
    viewMatrix.push(glm::mat4(1.0));
}

GPU_Matrices::~GPU_Matrices()
{
}

void GPU_Matrices::makeProjection3D(int fov, glm::vec2 depth)
{
    projectionMatrix.top() = glm::perspective((float)fov, 16.0f / 9.0f, depth.x, depth.y);
}

void GPU_Matrices::makeProjection2D()
{
    projectionMatrix.top() = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
}

void GPU_Matrices::translate(glm::vec3 translateVector)
{
    normalMatrix.top() = glm::translate(normalMatrix.top(), translateVector);
    modelMatrix.top() = glm::translate(modelMatrix.top(), translateVector);
}

void GPU_Matrices::rotate(float angle, glm::vec3 axis)
{
    normalMatrix.top() = glm::rotate(normalMatrix.top(), angle, axis);
    modelMatrix.top() = glm::rotate(modelMatrix.top(), angle, axis);
}

void GPU_Matrices::scale(glm::vec3 scaleVector)
{
    modelMatrix.top() = glm::scale(modelMatrix.top(), scaleVector);
}

void GPU_Matrices::push()
{
    normalMatrix.push(normalMatrix.top());
    modelMatrix.push(modelMatrix.top());
}

void GPU_Matrices::pop()
{
    normalMatrix.pop();
    modelMatrix.pop();
}

glm::mat4& GPU_Matrices::getProjectionMatrix()
{
    return projectionMatrix.top();
}

glm::mat4& GPU_Matrices::getNormalMatrix()
{
    return normalMatrix.top();
}

glm::mat4& GPU_Matrices::getModelMatrix()
{
    return modelMatrix.top();
}

glm::mat4& GPU_Matrices::getViewMatrix()
{
    return viewMatrix.top();
}