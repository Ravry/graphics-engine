#pragma once
#include <glad/glad.h>

class VAO {
private:
    unsigned int id;
public:
    VAO();
    void bind();
    void attrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    void unbind();
    void destroy();
};