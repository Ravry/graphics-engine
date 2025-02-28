#pragma once
#include <glad/glad.h>
#include <vector>

class VBO {
private:
    unsigned int id;
public:
    VBO();
    void bind();
    void data(std::vector<float>& data);
    void data(const void* data, GLsizeiptr size, GLenum usage);
    void unbind();
    void destroy();
};