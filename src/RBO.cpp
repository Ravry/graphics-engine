#include "headers/gl_classes/RBO.hpp"

RBO::RBO(int width, int height) {
    glGenRenderbuffers(1, &id);
    bind();
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
    unbind();
}

void RBO::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RBO::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::destroy() {

}