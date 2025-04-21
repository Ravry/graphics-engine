#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderInput {
    const char* vertexFile;
    const char* fragmentFile; 
    const char* geometryFile;
};

class Shader {
private:
    ShaderInput info;
    int getUniformLocation(const char* uniform);
public:
    unsigned int id;
    Shader(const ShaderInput& input);
    Shader* use();
    void unuse();
    void recompile();
    void destroy();

    Shader* SetInt(const char* uniform, int value);
    Shader* SetFloat(const char* uniform, float value);
    Shader* SetVector2(const char* uniform, glm::vec2 value);
    Shader* SetVector3(const char* uniform, glm::vec3 value);
    Shader* SetMatrix4x4(const char* uniform, glm::mat4 value);
};
