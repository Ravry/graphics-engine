#include "headers/gl_classes/Texture.hpp"

Texture::Texture(const char* filename, GLint filtermode) : target(GL_TEXTURE_2D) {
    glGenTextures(1, &id);
    bind();

	glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, filtermode);
	glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, filtermode);
    
    load_from_file(filename);

    unbind();
}

Texture::Texture(std::vector<std::string>& textures_faces) : target(GL_TEXTURE_CUBE_MAP) {
    glGenTextures(1, &id);
    bind();

    int width, height, nrChannels;
    unsigned char *data;  
    for(unsigned int i = 0; i < textures_faces.size(); i++)
    {
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}

Texture::Texture(int width, int height, GLenum target, GLint internalformat, GLint format, GLenum type) : target(target) {
    glGenTextures(1, &id);
    bind();

    if (this->target == GL_TEXTURE_2D_MULTISAMPLE) {
        glTexImage2DMultisample(this->target, 4, format, width, height, GL_TRUE);
    }
    else {
        glTexImage2D(this->target, 0, internalformat, width, height, 0,  format, type, NULL);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

Texture::Texture(int width, int height, GLenum target, const void* pixels) {
    this->target = target;

    glGenTextures(1, &id);
    bind();
    glTexImage2D(
        this->target,
        0,
        GL_RED,
        width,
        height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        pixels
    );

    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind() {
	glBindTexture(this->target, id);
}

void Texture::load_from_file(const char* filename) {
    int width, height, nrChannels;
    unsigned char* data;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format;
        switch (nrChannels) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default:
            std::cout << "Unsupported number of channels" << std::endl;
            stbi_image_free(data);
            return;
        }

        if (data != nullptr) {
            glTexImage2D(this->target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(this->target);
        }
        else {
            std::cout << "Error: Image data is null." << std::endl;
        }
    }
    else {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);
}

void Texture::unbind() {
	glBindTexture(this->target, 0);
}

void Texture::destroy() {
    glDeleteTextures(1, &id);
}