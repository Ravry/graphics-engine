#pragma once
#include <memory>
#include "gl_classes/Shader.hpp"
#include "gl_classes/Texture.hpp"
#include "gl_classes/FBO.hpp"
#include "Object.hpp"
#include "FontRenderer.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"

class Renderer {
public:
    unsigned int gBufferIndex = 0;

    Renderer(float width, float height);
    void update(const Input& input, float deltaTime);
    void render(float width, float height, float deltaTime);
    void refactor(float width, float height);
    void recompileShaders();
    void cleanup();
};