#pragma once
#include <vector>
#include "UI_ELEMENT.hpp"

namespace UI {
    class PANEL : public UI_ELEMENT {
        public:
            float padding;
            std::vector<UI_ELEMENT> panel_content;
            PANEL(glm::vec2 pos, glm::vec2 size, float padding);
            void render(Shader& shader, Camera& cam);
    };
};