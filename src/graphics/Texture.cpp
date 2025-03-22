#include "gl_classes/Texture.hpp"

Texture::Texture(const char* filename, GLint filtermode) : target(GL_TEXTURE_2D) {
    generateTexture();
    createTextureFromFile(filename, filtermode);
}

Texture::Texture(int width, int height, GLenum target, GLint internalformat, GLint format, GLenum type) : target(target) {
    generateTexture();
    createTexture(width, height, internalformat, format, type);
}

Texture::Texture(int width, int height, GLenum target, const void* pixels) : target(target) {
    generateTexture();
    createTexture(width, height, pixels);
}

Texture::Texture(std::vector<std::string>& textures_faces) : target(GL_TEXTURE_CUBE_MAP) {
    generateTexture();
    createCubemap(textures_faces);
}

Texture::Texture(const char* cubemapFile) : target(GL_TEXTURE_CUBE_MAP) {
    generateTexture();
    createCubemap(cubemapFile);
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

void extractFace(unsigned char* srcData, unsigned char* dstData, int fullWidth, int fullHeight, 
    int channels, int faceX, int faceY, int faceWidth, int faceHeight) {
    for (int y = 0; y < faceHeight; y++) {
        for (int x = 0; x < faceWidth; x++) {
            int srcPos = ((faceY * faceHeight + y) * fullWidth + (faceX * faceWidth + x)) * channels;
            int dstPos = (y * faceWidth + x) * channels;
            for (int c = 0; c < channels; c++) {
                dstData[dstPos + c] = srcData[srcPos + c];
            }
        }
    }
}

void Texture::generateTexture() {
    glGenTextures(1, &id);
    bind();
}

void Texture::createCubemap(std::vector<std::string>& textures_faces) {
    int width, height, nrChannels;
    unsigned char *data;  
    for(unsigned int i = 0; i < textures_faces.size(); i++)
    {
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
        );
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}

void Texture::createCubemap(const char* cubemapFile) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(cubemapFile, &width, &height, &nrChannels, 0);
    if (data) {
        int faceWidth = width / 4;
        int faceHeight = height / 3;
        unsigned char* faceData = new unsigned char[faceWidth * faceHeight * nrChannels];
        
        extractFace(data, faceData, width, height, nrChannels, 2, 1, faceWidth, faceHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, faceWidth, faceHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
        
        extractFace(data, faceData, width, height, nrChannels, 0, 1, faceWidth, faceHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, faceWidth, faceHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
        
        extractFace(data, faceData, width, height, nrChannels, 1, 0, faceWidth, faceHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, faceWidth, faceHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
        
        extractFace(data, faceData, width, height, nrChannels, 1, 2, faceWidth, faceHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, faceWidth, faceHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
        
        extractFace(data, faceData, width, height, nrChannels, 1, 1, faceWidth, faceHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, faceWidth, faceHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
        
        extractFace(data, faceData, width, height, nrChannels, 3, 1, faceWidth, faceHeight);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, faceWidth, faceHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
        
        delete[] faceData;
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    stbi_image_free(data);
}

void Texture::createTextureFromFile(const char *filename, GLenum filtermode)
{
	glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, filtermode);
	glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, filtermode);
    load_from_file(filename);
    unbind();
}

void Texture::createTexture(int width, int height, const void* pixels)
{
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

void Texture::createTexture(int width, int height, GLint internalformat, GLint format, GLenum type) {
    if (this->target == GL_TEXTURE_2D_MULTISAMPLE) {
        glTexImage2DMultisample(this->target, 4, format, width, height, GL_TRUE);
    }
    else {
        glTexImage2D(this->target, 0, internalformat, width, height, 0,  format, type, NULL);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}




void Texture::bind() {
	glBindTexture(this->target, id);
}

void Texture::bind(GLenum texture) {
    glActiveTexture(texture);
	glBindTexture(this->target, id);
}

void Texture::unbind() {
	glBindTexture(this->target, 0);
}

void Texture::destroy() {
    glDeleteTextures(1, &id);
}