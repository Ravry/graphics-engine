#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    unsigned int id;
    const char* vertexFile;
    const char* fragmentFile;
    int getUniformLocation(const char* uniform);
public:
    Shader(const char* vertexFile, const char* fragmentFile);
    void use();
    void unuse();
    void recompile();
    void destroy();

    void SetInt(const char* uniform, int value);
    void SetFloat(const char* uniform, float value);
    void SetVector2(const char* uniform, glm::vec2 value);
    void SetVector3(const char* uniform, glm::vec3 value);
    void SetMatrix4x4(const char* uniform, glm::mat4 value);
};
