#pragma once
#include "Texture.hpp"
#include <memory>
#include "../Camera.hpp"

class FBO {
private:
    GLenum target;
public:
    unsigned int id;
    std::unique_ptr<Texture> gAlbedo;
    std::unique_ptr<Texture> gDepth;
    std::unique_ptr<Texture> gPosition;
    std::unique_ptr<Texture> gNormal;
    std::unique_ptr<Texture> gMaterial;

    FBO(int width, int height, GLenum target);
    void bind(GLenum target);
    void blit(std::shared_ptr<Camera>& camera, GLenum source);
    void unbind();
    void refactor(int width, int height);
    void destroy();
};