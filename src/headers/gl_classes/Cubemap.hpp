#pragma once
#include "Texture.hpp"
#include <memory>

class Cubemap {
public:
    std::unique_ptr<Texture> texture;
    Cubemap(std::vector<std::string>& texture_faces);
};