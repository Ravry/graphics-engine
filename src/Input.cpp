#include "headers/Input.hpp"


void Input::update(GLFWwindow* window) {
    w_key = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)       ? 1 : 0;
    a_key = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)     ? 1 : 0;
    s_key = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)     ? 1 : 0;
    d_key = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)    ? 1 : 0;
    space_key = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)                                                  ? 1 : 0;
    ctrl_key = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)                                            ? 1 : 0;
    glfwGetCursorPos(window, &x_mouse, &y_mouse);
}