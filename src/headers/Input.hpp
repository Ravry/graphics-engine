#pragma once
#include <GLFW/glfw3.h>

class Input {
public:
    float w_key;
    float a_key;
    float s_key;
    float d_key;
    float space_key;
    float ctrl_key;

    double x_mouse;
    double y_mouse;

    void update(GLFWwindow* window);
};