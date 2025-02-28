#pragma once
#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"

class Window {
private:
    GLFWwindow* window;
public:
    float width;
    float height;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Input> input;

    Window(const char* title, const int width, const int height);
    void set_icon(const char* filename);
    void run();
    void destroy();
};