#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include "stb_image.h"

class Texture {
private:
    GLenum target;
    void load_from_file(const char* filename);
public:
    unsigned int id;
    Texture(const char* filename, GLint filtermode);
    Texture(std::vector<std::string>& textures_faces);
    Texture(int width, int height, GLenum target, GLint internalformat, GLint format, GLenum type = GL_UNSIGNED_BYTE);
    Texture(int width, int heigth, GLenum target, const void* pixels);
    void bind();
    void unbind();
    void destroy();
};