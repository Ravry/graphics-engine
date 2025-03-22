#pragma once
#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


class Window {
private:
    GLFWwindow* window;
public:
    float width;
    float height;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Input> input;
    bool focused {true};

    Window(const char* title, const int width, const int height);
    void set_icon(const char* filename);
    void run();
    void destroy();
};