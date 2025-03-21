#include "gl_classes/VBO.hpp"

VBO::VBO() {
    glGenBuffers(1, &id);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::data(std::vector<float>& data) {
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
}


void VBO::data(const void* data, GLsizeiptr size, GLenum usage) {
    glBufferData(GL_ARRAY_BUFFER, size,data, usage);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy() {
    glDeleteBuffers(1, &id);
}