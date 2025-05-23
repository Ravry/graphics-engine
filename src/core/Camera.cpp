#include "Camera.hpp"

Camera::Camera(float width, float height) : Transform(), 
    projection(glm::perspective(glm::radians(60.f), width/height, .1f, 1000.f)), 
    ortho(glm::ortho(0.f, width, 0.f, height)),
    position(glm::vec3(0)),
    worldUP(glm::vec3(0, 1, 0)),
    yaw(-90.f),
    pitch(0.f),
    lastX(width / 2.f),
    lastY(height / 2.f),
    sens(5.f),
    speed(40.f),
    width(width),
    height(height)
{}

void Camera::handleMouseInput(const Input& input, float deltaTime) {
    float xpos = input.x_mouse;
    float ypos = input.y_mouse;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sens * deltaTime;
    yoffset *= sens * deltaTime;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 _front;
    _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _front.y = sin(glm::radians(pitch));
    _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(_front);
    right = glm::normalize(glm::cross(front, worldUP));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::handleKeyboardInput(const Input& input, float deltaTime) {
    glm::vec3 _input = glm::vec3(0);
    _input.x = input.d_key - input.a_key;
    _input.y = input.space_key - input.ctrl_key;
    _input.z = input.w_key - input.s_key;

    if (glm::length(_input) > 0.f)
        _input = glm::normalize(_input);

    position += (_input.x * right + _input.y * worldUP + _input.z * front) * speed * deltaTime;
}

void Camera::update(bool windowFocused, const Input& input, float deltaTime) {
    if (!windowFocused)
        return;
        
    handleMouseInput(input, deltaTime);
    handleKeyboardInput(input, deltaTime);
    matrix = glm::lookAt(position, position + front, up);
}

void Camera::refactor(float width, float height) {
    projection = glm::perspective(glm::radians(60.f), width/height, .1f, 100.f);
    ortho = glm::ortho(0.f, width, 0.f, height);
    this->width = width;
    this->height = height;
}