#include "headers/gl_classes/Cubemap.hpp"

Cubemap::Cubemap(std::vector<std::string>& texture_faces) {
    texture = std::make_unique<Texture>(texture_faces);
}



