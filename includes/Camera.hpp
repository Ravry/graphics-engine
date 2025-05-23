#pragma once
#include <iostream>
#include <memory>
#include "Transform.hpp"
#include "Input.hpp"

class Camera : public Transform {
private:
    float yaw, pitch;

    void handleMouseInput(const Input& input, float deltaTime);
    void handleKeyboardInput(const Input& input, float deltaTime);
public:
    float sens;
    float speed;
    float lastX, lastY;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUP;
    glm::mat4 projection;
    glm::mat4 ortho;
    float width, height;

    Camera(float width, float height);
    void update(bool windowFocused, const Input& input, float deltaTime);
    void refactor(float width, float height);
};