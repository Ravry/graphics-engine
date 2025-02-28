#include "headers/IMAGE.hpp"

UI::IMAGE::IMAGE(glm::vec2 pos, glm::vec2 size) : UI::UI_ELEMENT(pos, size) {
    Geometry::Geom quadGeom = Geometry::quadGeom();

    translate(glm::vec3(pos, 0));
    scale(glm::vec3(size, 1));

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>();
    ebo = std::make_unique<EBO>();

    vao->bind();
    vbo->bind();
    vbo->data(quadGeom.vertices);
    vao->attrib(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    vao->attrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));

    ebo->bind();
    ebo->data(quadGeom.indices);

    // vbo->unbind();
    vao->unbind();
    ebo->unbind();
}

void UI::IMAGE::render(Camera& camera, Shader& shader) {
    setPosition(glm::vec3(camera.width/2, camera.height/2, 0));
    // setRotation(glm::vec3(0, 0, 1), 60);
    // setScale(glm::vec3(200, 200, 1));
    vao->bind();
    shader.use();
    shader.SetMatrix4x4("projection", camera.ortho);
    shader.SetMatrix4x4("model", matrix);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    shader.unuse();
    vao->unbind();
}