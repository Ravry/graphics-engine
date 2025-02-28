#include "headers/gl_classes/Shader.hpp"

std::string readFileContent(const char* filename) {
    std::string fileContent;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        file.close();
        fileContent = fileStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    return fileContent;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) : vertexFile(vertexFile), fragmentFile(fragmentFile) {
    std::string vertexCode = readFileContent(vertexFile);
    std::string fragmentCode = readFileContent(fragmentFile);

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::unuse() {
    glUseProgram(0);
}

void Shader::recompile() {
    destroy();

    std::string vertexCode = readFileContent(vertexFile);
    std::string fragmentCode = readFileContent(fragmentFile);

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::destroy() {
    glDeleteProgram(id);
}

int Shader::getUniformLocation(const char* uniform) {
    return glGetUniformLocation(id, uniform);
}

void Shader::SetInt(const char* uniform, int value) {
    int location = getUniformLocation(uniform);
    glUniform1i(location, value);
}

void Shader::SetFloat(const char* uniform, float value) {\
    int location = getUniformLocation(uniform);
    glUniform1f(location, value); 
}

void Shader::SetVector2(const char* uniform, glm::vec2 value)  {
    int location = getUniformLocation(uniform);
    glUniform2f(location, value.x, value.y);
}

void Shader::SetVector3(const char* uniform, glm::vec3 value) {
    int location = getUniformLocation(uniform);
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetMatrix4x4(const char* uniform, glm::mat4 value)  {
    int location = getUniformLocation(uniform);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}