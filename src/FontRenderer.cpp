#include "headers/FontRenderer.hpp"


FontRenderer::FontRenderer(const char* fontfile, const int fontsize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, fontfile, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontsize);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        std::shared_ptr<Texture> charTex = std::make_shared<Texture>(face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_TEXTURE_2D, face->glyph->bitmap.buffer);

        Character character = {
            charTex,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    shader = std::make_unique<Shader>("resources/shader/text/text.vert", "resources/shader/text/text.frag");

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>();
    ebo = std::make_unique<EBO>();

    vao->bind();

    vbo->bind();

    std::vector<float> vertices {
        0.f,     0.f,    0.f,          0.f, 1.f,  // Bottom-left  (flip V)
        1.f,     0.f,    0.f,          1.f, 1.f,  // Bottom-right (flip V)
        1.f,     1.f,    0.f,          1.f, 0.f,  // Top-right    (flip V)
        0.f,     1.f,    0.f,          0.f, 0.f   // Top-left     (flip V)
   };   

    std::vector<unsigned int> indices {
        0, 1, 2,
        2, 3, 0  
    };

    vbo->data(vertices);

    vao->attrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    vao->attrib(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    ebo->bind();

    ebo->data(indices);

    vbo->unbind();
    vao->unbind();
    ebo->unbind();
}

void FontRenderer::render(std::string text, glm::mat4 projection, glm::vec2 position, glm::vec3 color, float scale) {
    shader->use();
    shader->SetMatrix4x4("projection", projection);
    shader->SetFloat("time", glfwGetTime());

    vao->bind();

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = position.x + ch.Bearing.x * scale;
        float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1), glm::vec3(xpos, ypos, 0.f)), glm::vec3(w, h, 1.f));
        
        ch.texture->bind();
        shader->SetMatrix4x4("model", model);
        shader->SetVector3("fontColor", color);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        position.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    vao->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}