#include "gl_classes/VAO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::bind() {
    glBindVertexArray(id);
}

void VAO::attrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::destroy() {
    glDeleteVertexArrays(1, &id);
}