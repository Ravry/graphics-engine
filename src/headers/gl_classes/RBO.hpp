#pragma once
#include <glad/glad.h>

class RBO {
public:
    unsigned int id;
    RBO(int width, int height);
    void bind();
    void unbind();
    void destroy();
};