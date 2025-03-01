#pragma once
#include "Transform.hpp"
#include "gl_classes/Shader.hpp"
#include "Camera.hpp"

namespace UI {
    class UI_ELEMENT : public Transform {
    public:
        glm::vec2 position;
        glm::vec2 size;
        UI_ELEMENT(glm::vec2 pos, glm::vec2 size);
    };
}