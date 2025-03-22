#pragma once
#include <iostream>
#include <memory>
#include <thread>
#include <queue>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "gl_classes/Shader.hpp"
#include "gl_classes/Texture.hpp"
#include "gl_classes/FBO.hpp"
#include "Object.hpp"
#include "FontRenderer.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ComponentSomething.hpp"
#include "Chemisty.hpp"

class Renderer {
public:
    unsigned int gBufferIndex = 0;
    std::shared_ptr<Camera> camera;

    Renderer(float width, float height);
    void update(bool windowFocused, const Input& input, float deltaTime);
    void imgui();
    void render(float width, float height, float deltaTime);
    void refactor(float width, float height);
    void recompileShaders();
    void cleanup();
};