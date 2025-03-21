#include "Object.hpp"

Object::Object(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::vector<Attrib>& attribs) : tri_count(indices.size()), Transform() {
    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>();
    ebo = std::make_unique<EBO>();

    vao->bind();
    vbo->bind();
    vbo->data(vertices);

    for (auto attrib : attribs)
        vao->attrib(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer);
    
    ebo->bind();
    ebo->data(indices);
    vbo->unbind();
    vao->unbind();
    ebo->unbind();
}

Object::Object(std::vector<Mesh>& meshes, std::vector<Attrib>& attribs) : Transform() {
    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>();
    ebo = std::make_unique<EBO>();
    bonesVBO = std::make_unique<VBO>();

    vao->bind();
    vbo->bind();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<VertexBoneData> bones;
    GLuint offset = 0;

    for (const auto& mesh : meshes) {
        vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
        bones.insert(bones.end(), mesh.bones.begin(), mesh.bones.end());

        for (auto& index : mesh.indices) {
            indices.push_back(index + offset);
        }

        offset += mesh.vertices.size() / 8;
    }
    
    vbo->data(vertices);

    for (auto attrib : attribs)
        vao->attrib(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer);

    bonesVBO->bind();
    bonesVBO->data(bones.data(), bones.size() * sizeof(VertexBoneData), GL_STATIC_DRAW);

    vao->attrib(3, 4, GL_INT, GL_FALSE, sizeof(VertexBoneData), (void*)0); // Bone IDs
    vao->attrib(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)(4 * sizeof(int))); // Weights

    ebo->bind();
    ebo->data(indices);

    vbo->unbind();
    bonesVBO->unbind();
    vao->unbind();
    ebo->unbind();

    tri_count = indices.size();
}

void Object::render(std::function<void()> render_func) {
    render_func();
    vao->bind();
    glDrawElements(GL_TRIANGLES, tri_count, GL_UNSIGNED_INT, 0);
    vao->unbind();
}

void Object::destroy() {
    vbo->destroy();
    ebo->destroy();
    vao->destroy();
}