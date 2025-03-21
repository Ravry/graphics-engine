#include "gl_classes/FBO.hpp"

FBO::FBO(int width, int height, GLenum target) : target(target) {
    glActiveTexture(GL_TEXTURE0);

    glGenFramebuffers(1, &id);
    bind(GL_FRAMEBUFFER);
    
    this->gAlbedo = std::make_unique<Texture>(width, height, this->target, GL_RGB, GL_RGB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->target, this->gAlbedo->id, 0);

    this->gDepth = std::make_unique<Texture>(width, height, this->target, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->target, this->gDepth->id, 0);

    this->gPosition = std::make_unique<Texture>(width, height, this->target, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->target, this->gPosition->id, 0);

    // this->gNormal = new Texture(width, height, GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE);
    this->gNormal = std::make_unique<Texture>(width, height, this->target, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, this->target, this->gNormal->id, 0);

    this->gMaterial = std::make_unique<Texture>(width, height, this->target, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, this->target, this->gMaterial->id, 0);

    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    unbind();
}

void FBO::bind(GLenum target) {
    glBindFramebuffer(target, id);  
}

void FBO::blit(std::shared_ptr<Camera>& camera, GLenum source) {
    glReadBuffer(source);
    glDrawBuffer(source);
    glBlitFramebuffer(0, 0, camera->width, camera->height, 0, 0, camera->width, camera->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);    
}

void FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::refactor(int width, int height) {
    gAlbedo->destroy();
    gDepth->destroy();
    gPosition->destroy();
    gNormal->destroy();

    bind(GL_FRAMEBUFFER);
    
    this->gAlbedo = std::make_unique<Texture>(width, height, this->target, GL_RGB, GL_RGB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->target, this->gAlbedo->id, 0);

    this->gDepth = std::make_unique<Texture>(width, height, this->target, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->target, this->gDepth->id, 0);

    this->gPosition = std::make_unique<Texture>(width, height, this->target, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->target, this->gPosition->id, 0);

    this->gNormal = std::make_unique<Texture>(width, height, this->target, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, this->target, this->gNormal->id, 0);

    this->gMaterial = std::make_unique<Texture>(width, height, this->target, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, this->target, this->gMaterial->id, 0);

    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    unbind();
}

void FBO::destroy() {
    glDeleteFramebuffers(1, &id);
}