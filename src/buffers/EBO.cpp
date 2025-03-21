#include "gl_classes/EBO.hpp"

EBO::EBO() {
    glGenBuffers(1, &id);
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::data(std::vector<unsigned int>& data) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy() {
    glDeleteBuffers(1, &id);
}