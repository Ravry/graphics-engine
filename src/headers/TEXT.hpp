#pragma once
#include "UI_ELEMENT.hpp"

namespace UI {
    class TEXT : public UI_ELEMENT {
        public:
            const char* text;
            TEXT(glm::vec2 pos, glm::vec2 size);
            void render();
    };
};