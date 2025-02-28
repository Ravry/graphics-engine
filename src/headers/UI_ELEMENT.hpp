#pragma once
#include "Transform.hpp"

namespace UI {
    class UI_ELEMENT : public Transform {
    public:
        glm::vec2 position;
        glm::vec2 size;
        UI_ELEMENT(glm::vec2 pos, glm::vec2 size);
    };
}