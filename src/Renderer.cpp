#include "headers/Renderer.hpp"

std::unique_ptr<Camera> camera;
std::unique_ptr<Shader> defaultShader;
std::unique_ptr<Shader> cubemapShader;
std::unique_ptr<Shader> screenShader;
std::unique_ptr<Shader> uiShader;
std::unique_ptr<Texture> texture;
std::unique_ptr<Cubemap> cubemap;
std::unique_ptr<Object> cubemapObject;
std::unique_ptr<Object> quadObject;
std::unique_ptr<Object> sphereObject;
std::unique_ptr<Object> modelObject;
std::unique_ptr<FBO> fbo;
std::unique_ptr<FBO> intermediateFBO;
std::unique_ptr<FontRenderer> fontRenderer;
std::unique_ptr<UI::IMAGE> ui_image;

std::map<int, std::string> gBufferTextures {
    { 0, "g_albedo" },
    { 1, "g_normal" },
    { 2, "g_position" },
    { 3, "g_material" },
    { 4, "g_depth" }
};

Renderer::Renderer(float width, float height) {
    glActiveTexture(GL_TEXTURE0);
    texture = std::make_unique<Texture>("resources/texture/checkered.png", GL_NEAREST);
    camera = std::make_unique<Camera>(width, height);
    defaultShader = std::make_unique<Shader>("resources/shader/default/default.vert", "resources/shader/default/default.frag");

    std::vector<Attrib> cubeAttribs {
        Attrib {
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            0
        }
    };
    Geometry::Geom cubeGeom = Geometry::createCube();
    cubemapObject = std::make_unique<Object>(cubeGeom.vertices, cubeGeom.indices, cubeAttribs);
    std::vector<std::string> texture_faces {
        "resources/texture/cubemap/skybox/right.png",
        "resources/texture/cubemap/skybox/left.png",
        "resources/texture/cubemap/skybox/top.png",
        "resources/texture/cubemap/skybox/bottom.png",
        "resources/texture/cubemap/skybox/front.png",
        "resources/texture/cubemap/skybox/back.png"
    };
    glActiveTexture(GL_TEXTURE1);
    cubemap = std::make_unique<Cubemap>(texture_faces);
    cubemapShader = std::make_unique<Shader>("resources/shader/cubemap/cubemap.vert", "resources/shader/cubemap/cubemap.frag");
    
    std::vector<Attrib> defaultAttribs {
        Attrib {
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            0
        },
        Attrib {
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*)(3 * sizeof(float))
        },
        Attrib {
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*)(5 * sizeof(float))
        }
    };
    
    Geometry::Geom quadGeom = Geometry::quadGeom();
    quadObject = std::make_unique<Object>(quadGeom.vertices, quadGeom.indices, defaultAttribs);
    quadObject->scale(glm::vec3(20, 1, 20));
    quadObject->rotate(glm::vec3(1, 0, 0), -90.f);

    Geometry::Geom sphereGeom = Geometry::createSphere(8 * 2, 8 * 2, 1.f);
    sphereObject = std::make_unique<Object>(sphereGeom.vertices, sphereGeom.indices, defaultAttribs);
    sphereObject->translate(glm::vec3(0, 1.2f, 0));

    Model model = Model("resources/model/TempleRuin_02.DAE");
    modelObject = std::make_unique<Object>(model.meshes, defaultAttribs);

    screenShader = std::make_unique<Shader>("resources/shader/screen/screen.vert", "resources/shader/screen/screen.frag");
    
    uiShader = std::make_unique<Shader>("resources/shader/ui/ui.vert", "resources/shader/ui/ui.frag");

    glEnable(GL_MULTISAMPLE);  

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    fbo = std::make_unique<FBO>(width, height, GL_TEXTURE_2D_MULTISAMPLE);
    intermediateFBO = std::make_unique<FBO>(width, height, GL_TEXTURE_2D);
    
    fontRenderer = std::make_unique<FontRenderer>("resources/fonts/arial.ttf", 22);

    ui_image = std::make_unique<UI::IMAGE>(glm::vec2(0), glm::vec2(100.f));

    glEnable(GL_CULL_FACE);  
    glFrontFace(GL_CCW);  
}

void Renderer::update(const Input& input, float deltaTime) {
    camera->update(input, deltaTime);
}

void Renderer::render(float width, float height, float deltaTime) {
    fbo->bind(GL_FRAMEBUFFER);

    glClearColor(1.f, .0f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    defaultShader->use();
    defaultShader->SetMatrix4x4("projection", camera->projection);
    defaultShader->SetMatrix4x4("view", camera->matrix);
    defaultShader->SetMatrix4x4("model", quadObject->matrix);
    
    defaultShader->SetVector2("texTiling", glm::vec2(20));
    defaultShader->SetVector3("texTint", glm::vec3(1));
    defaultShader->SetFloat("metallic", 1.f);
    defaultShader->SetVector3("cameraPos", camera->position);
    defaultShader->SetInt("tex", 0);
    defaultShader->SetInt("cubemap", 1);
    
    quadObject->render();

    defaultShader->SetVector3("texTint", glm::vec3(0, .8, 0));
    defaultShader->SetFloat("metallic", 0.f);
    defaultShader->SetMatrix4x4("model", sphereObject->matrix);
    sphereObject->render();

    defaultShader->SetMatrix4x4("model", sphereObject->matrix);
    modelObject->render();
    texture->unbind();
    defaultShader->unuse();

    glDepthFunc(GL_LEQUAL);
    cubemap->texture->bind();
    cubemapShader->use();
    cubemapShader->SetMatrix4x4("projection", camera->projection);
    cubemapShader->SetMatrix4x4("view", glm::mat4(glm::mat3(camera->matrix)));
    cubemapShader->SetInt("cubemap", 1);
    cubemapObject->render();
    glDepthFunc(GL_LESS);
    
    fbo->bind(GL_READ_FRAMEBUFFER);
    intermediateFBO->bind(GL_DRAW_FRAMEBUFFER);
    
    fbo->blit(*camera, GL_COLOR_ATTACHMENT0);
    fbo->blit(*camera, GL_COLOR_ATTACHMENT1);
    fbo->blit(*camera, GL_COLOR_ATTACHMENT2);
    fbo->blit(*camera, GL_COLOR_ATTACHMENT3);
    
    glBlitFramebuffer(0, 0, camera->width, camera->height, 0, 0, camera->width, camera->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    
    fbo->unbind();

    glDisable(GL_DEPTH_TEST);

    glClearColor(1.f, .0f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    intermediateFBO->gAlbedo->bind();
    glActiveTexture(GL_TEXTURE1);
    intermediateFBO->gNormal->bind();
    glActiveTexture(GL_TEXTURE2);
    intermediateFBO->gPosition->bind();
    glActiveTexture(GL_TEXTURE3);
    intermediateFBO->gDepth->bind();
    glActiveTexture(GL_TEXTURE4);
    intermediateFBO->gMaterial->bind();

    screenShader->use();
    screenShader->SetInt("albedoTex", 0);
    screenShader->SetInt("positionTex", 2);
    screenShader->SetInt("normalTex", 1);
    screenShader->SetInt("depthTex", 3);
    screenShader->SetInt("materialTex", 4);
    screenShader->SetInt("gBufferTex", gBufferIndex);
    
    screenShader->SetMatrix4x4("projection", camera->projection);
    screenShader->SetMatrix4x4("view", camera->matrix);
    screenShader->SetVector3("cameraPos", camera->position);

    quadObject->render();

    intermediateFBO->gAlbedo->unbind();
    intermediateFBO->gDepth->unbind();
    
    glActiveTexture(GL_TEXTURE0);
    fontRenderer->render(std::to_string(int(1.0f/deltaTime)), camera->ortho, glm::vec2(10, height - 280 + 22));
    fontRenderer->render(gBufferTextures[gBufferIndex], camera->ortho, glm::vec2(10, height - 280));

    // ui_image->render(*camera, *uiShader);
}

void Renderer::recompileShaders() {
    defaultShader->recompile();
    cubemapShader->recompile();
    screenShader->recompile();
    uiShader->recompile();
}

void Renderer::refactor(float width, float height) {
    camera->refactor(width, height);
    fbo->refactor(width, height);
    intermediateFBO->refactor(width, height);
}

void Renderer::cleanup() {
    defaultShader->destroy();
    cubemapShader->destroy();
    screenShader->destroy();
    quadObject->destroy();
    cubemapObject->destroy();
    texture->destroy();
    cubemap->texture->destroy();
    fbo->destroy();
    intermediateFBO->destroy();
}