#pragma once
#include <iostream>
#include <map>
#include "Transform.hpp"
#include "gl_classes/VAO.hpp"
#include "gl_classes/VBO.hpp"
#include "gl_classes/EBO.hpp"
#include "gl_classes/Shader.hpp"
#include "gl_classes/Texture.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "glfw/glfw3.h"


struct Character {
    std::shared_ptr<Texture> texture;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class FontRenderer {
private:
    std::map<char, Character> Characters;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
public:
    FontRenderer(const char* fontfile, const int fontsize);
    void render(std::string text, glm::mat4 projection, glm::vec2 screenPos, glm::vec3 color = glm::vec3(1), float scale = 1.f);
};