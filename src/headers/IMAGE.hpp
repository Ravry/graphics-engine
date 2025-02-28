#pragma once
#include <memory>
#include "gl_classes/VAO.hpp"
#include "gl_classes/VBO.hpp"
#include "gl_classes/EBO.hpp"
#include "Geometry.hpp"
#include "UI_ELEMENT.hpp"
#include "FontRenderer.hpp"
#include "Camera.hpp"

namespace UI {
    class IMAGE : public UI_ELEMENT {
        private:
            std::unique_ptr<VAO> vao;
            std::unique_ptr<VBO> vbo;
            std::unique_ptr<EBO> ebo;
        public:
            const char* input;
            IMAGE(glm::vec2 pos, glm::vec2 size);
            void render(Camera& camera, Shader& shader);
    };
};