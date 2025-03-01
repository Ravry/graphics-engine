#include "headers/PANEL.hpp"

UI::PANEL::PANEL(glm::vec2 pos, glm::vec2 size, float padding) : UI_ELEMENT(pos, size), padding(padding) {

}

void UI::PANEL::render(Shader& shader, Camera& cam) {
    shader.use();
    
    shader.unuse();
}