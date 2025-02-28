#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Transform {
public:
    glm::mat4 matrix;
    
    Transform(glm::vec3 vec = glm::vec3(0), float rotationDeg = 0, glm::vec3 rotationAxis = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
    void translate(glm::vec3 vec);
    void rotate(glm::vec3 axis, float rotationDeg);
    void scale(glm::vec3 scalar);
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 axis, float rotationDeg);
    void setScale(glm::vec3 scalar);
};