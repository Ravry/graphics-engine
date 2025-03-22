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

    void generateTexture();
    void createCubemap(std::vector<std::string>& textures_faces);
    void createCubemap(const char* cubemapFile);
    void createTextureFromFile(const char* filename, GLenum filtermode);
    void createTexture(int width, int height, const void* pixels);
    void createTexture(int width, int height, GLint internalformat, GLint format, GLenum type);

public:
    unsigned int id;

    Texture(const char* filename, GLint filtermode);
    Texture(int width, int height, GLenum target, GLint internalformat, GLint format, GLenum type = GL_UNSIGNED_BYTE);
    Texture(int width, int heigth, GLenum target, const void* pixels);
    Texture(std::vector<std::string>& textures_faces);
    Texture(const char* cubemapFile);
    
    void bind();
    void bind(GLenum texture);
    void unbind();
    void destroy();
};