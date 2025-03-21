#pragma once
#include <glad/glad.h>
#include <vector>

class EBO {
private:
    unsigned int id;
public:
    EBO();
    void bind();
    void unbind();
    void destroy();
    void data(std::vector<unsigned int>& data);
};