#pragma once
#include <memory>
#include <functional>
#include "gl_classes/VAO.hpp"
#include "gl_classes/VBO.hpp"
#include "gl_classes/EBO.hpp"
#include "gl_classes/Shader.hpp"
#include "Transform.hpp"
#include "Model.hpp"
#include "Entity.hpp"

struct Attrib {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const void *pointer;
};

class Object : public Transform, public Entity {
private:
    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    unsigned int tri_count;
public:
    Object(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::vector<Attrib>& attribs);
    Object(std::vector<Mesh>& meshes, std::vector<Attrib>& attribs);
    ~Object();
    void render(std::function<void()> render_func);
    void renderInstanced(int instanceCount);
    void destroy();

    VAO* getVAO() {
        return vao.get();
    }
};