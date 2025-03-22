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

void VAO::attribInstanced(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
    glVertexAttribDivisor(index, 1);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::destroy() {
    glDeleteVertexArrays(1, &id);
}