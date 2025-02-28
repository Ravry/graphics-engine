#pragma once
#include <memory>
#include "gl_classes/VAO.hpp"
#include "gl_classes/VBO.hpp"
#include "gl_classes/EBO.hpp"
#include "Transform.hpp"
#include "Model.hpp"

struct Attrib {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const void *pointer;
};

class Object : public Transform {
private:
    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    unsigned int tri_count;
public:
    Object(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::vector<Attrib>& attribs);
    Object(std::vector<Mesh> meshes, std::vector<Attrib>& attribs);
    void render();
    void destroy();
};