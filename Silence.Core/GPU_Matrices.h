
#pragma once

#include "EngineLayer.h"
#include <stack>

class SILENCE_EXPORT GPU_Matrices
{
private:
    std::stack<glm::mat4> projectionMatrix;
    std::stack<glm::mat4> normalMatrix;
    std::stack<glm::mat4> modelMatrix;
    std::stack<glm::mat4> viewMatrix;
public:
    GPU_Matrices();
    ~GPU_Matrices();

    void makeProjection3D(int fov, glm::vec2 depth);
    void makeProjection2D();

    void translate(glm::vec3 translateVector);
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 scaleVector);
    void push();
    void pop();

    glm::mat4& getProjectionMatrix();
    glm::mat4& getNormalMatrix();
    glm::mat4& getModelMatrix();
    glm::mat4& getViewMatrix();
};

typedef GPU_Matrices Matrices;