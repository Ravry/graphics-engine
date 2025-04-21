#include "Renderer.hpp"

std::map<std::string, std::shared_ptr<Shader>> shaders;
std::map<std::string, std::shared_ptr<Texture>> textures;
std::map<std::string, std::shared_ptr<Object>> objects;
std::map<int, std::string> gBufferTextures {
    { 0, "g_albedo" },
    { 1, "g_normal" },
    { 2, "g_position" },
    { 3, "g_material" },
    { 4, "g_depth" }
};

std::unique_ptr<Object> atomSphereObj;
std::unique_ptr<Object> bondCylinderObj;

std::shared_ptr<FBO> fbo;
std::shared_ptr<FBO> intermediateFBO;
std::shared_ptr<FontRenderer> fontRenderer;

std::vector<std::string> texture_faces {
    "resources/texture/cubemap/skybox_1/right.png",
    "resources/texture/cubemap/skybox_1/left.png",
    "resources/texture/cubemap/skybox_1/top.png",
    "resources/texture/cubemap/skybox_1/bottom.png",
    "resources/texture/cubemap/skybox_1/front.png",
    "resources/texture/cubemap/skybox_1/back.png"
};

ParticleSystem particleSystem;
VAO* vao;
VBO* vbo;

float randomFloat(float min, float max) {
    static std::random_device rd;  // Obtain a random number generator
    static std::mt19937 gen(rd()); // Initialize with a random seed
    std::uniform_real_distribution<> dis(min, max); // Uniform distribution
    return dis(gen);
}

Renderer::Renderer(float width, float height) {
    /**
    * ███████╗██╗  ██╗ █████╗ ██████╗ ███████╗██████╗ ███████╗
    * ██╔════╝██║  ██║██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝
    * ███████╗███████║███████║██║  ██║█████╗  ██████╔╝███████╗
    * ╚════██║██╔══██║██╔══██║██║  ██║██╔══╝  ██╔══██╗╚════██║
    * ███████║██║  ██║██║  ██║██████╔╝███████╗██║  ██║███████║
    * ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚══════╝
    */
    shaders["defaultShader"] = std::make_shared<Shader>(ShaderInput {"resources/shader/default/default.vert", "resources/shader/default/default.frag", nullptr});
    shaders["cubemapShader"] = std::make_shared<Shader>(ShaderInput {"resources/shader/cubemap/cubemap.vert", "resources/shader/cubemap/cubemap.frag", nullptr});
    shaders["screenShader"] = std::make_shared<Shader>(ShaderInput {"resources/shader/screen/screen.vert", "resources/shader/screen/screen.frag", nullptr});
    shaders["uiShader"] = std::make_shared<Shader>(ShaderInput {"resources/shader/ui/ui.vert", "resources/shader/ui/ui.frag", nullptr});
    shaders["instancingShader"] = std::make_shared<Shader>(ShaderInput {"resources/shader/instancing/instancing.vert", "resources/shader/instancing/instancing.frag", nullptr});
    shaders["particleShader"] = std::make_shared<Shader>(ShaderInput {"resources/shader/particles/particles.vert", "resources/shader/particles/particles.frag", "resources/shader/particles/particles.geom"});
    
    const size_t PARTICLE_COUNT = 1000000;
    for (size_t i {0}; i < PARTICLE_COUNT; i++) {
        particleSystem.points.push_back(randomFloat(-500.f, 500.f));
        particleSystem.points.push_back(randomFloat( 0.f, 100.f));
        particleSystem.points.push_back(randomFloat(-500.f, 500.f));
    }

    vao = new VAO();
    vao->bind();
    vbo = new VBO();
    vbo->bind();
    vbo->data(particleSystem.points);
    vao->attrib(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    vao->unbind();
    vbo->unbind();

    /**
    * ████████╗███████╗██╗  ██╗████████╗██╗   ██╗██████╗ ███████╗███████╗
    * ╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██║   ██║██╔══██╗██╔════╝██╔════╝
    *    ██║   █████╗   ╚███╔╝    ██║   ██║   ██║██████╔╝█████╗  ███████╗
    *    ██║   ██╔══╝   ██╔██╗    ██║   ██║   ██║██╔══██╗██╔══╝  ╚════██║
    *    ██║   ███████╗██╔╝ ██╗   ██║   ╚██████╔╝██║  ██║███████╗███████║
    *    ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝
    */
    textures["checkered"] = std::make_shared<Texture>("resources/texture/checkered.png", GL_NEAREST);
    textures["cubemap"] = std::make_shared<Texture>("resources/texture/cubemap/skybox_2/cubemap.png");

    /**
    *  ██████╗ ██████╗      ██╗███████╗ ██████╗████████╗███████╗
    * ██╔═══██╗██╔══██╗     ██║██╔════╝██╔════╝╚══██╔══╝██╔════╝
    * ██║   ██║██████╔╝     ██║█████╗  ██║        ██║   ███████╗
    * ██║   ██║██╔══██╗██   ██║██╔══╝  ██║        ██║   ╚════██║
    * ╚██████╔╝██████╔╝╚█████╔╝███████╗╚██████╗   ██║   ███████║
    *  ╚═════╝ ╚═════╝  ╚════╝ ╚══════╝ ╚═════╝   ╚═╝   ╚══════╝
    */
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
    
    Geometry::Geom cubeGeom = Geometry::createCube();
    Geometry::Geom quadGeom = Geometry::quadGeom();
    Geometry::Geom sphereGeom = Geometry::createSphere(8 * 2, 8 * 2, 1.f);
    Geometry::Geom cylinderGeom = Geometry::createCylinder(16, 1.0f, 1.f);
    
    
    objects["cubemapObject"] = std::make_shared<Object>(cubeGeom.vertices, cubeGeom.indices, cubeAttribs);
    objects["quadObject"] = std::make_shared<Object>(quadGeom.vertices, quadGeom.indices, defaultAttribs);
    objects["quadObject"]->scale(glm::vec3(500, 1, 500))->rotate(glm::vec3(1, 0, 0), -90.f);
    objects["sphereObject"] = std::make_shared<Object>(sphereGeom.vertices, sphereGeom.indices, defaultAttribs);
    objects["sphereObject"]->scale(glm::vec3(2.f))->translate(glm::vec3(0, 2.f, 0));        
    atomSphereObj = std::make_unique<Object>(sphereGeom.vertices, sphereGeom.indices, defaultAttribs);
    bondCylinderObj = std::make_unique<Object>(cylinderGeom.vertices, cylinderGeom.indices, defaultAttribs);
            
    
    [] {
        glEnable(GL_MULTISAMPLE);  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);      
    }();

    fbo = std::make_shared<FBO>(width, height, GL_TEXTURE_2D_MULTISAMPLE);
    intermediateFBO = std::make_shared<FBO>(width, height, GL_TEXTURE_2D);
    
    fontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", 22);
    camera = std::make_shared<Camera>(width, height);
}

void Renderer::update(bool windowFocused, const Input& input, float deltaTime) {
    camera->update(windowFocused, input, deltaTime);
}

/**
* ██╗███╗   ███╗ ██████╗ ██╗   ██╗██╗
* ██║████╗ ████║██╔════╝ ██║   ██║██║
* ██║██╔████╔██║██║  ███╗██║   ██║██║
* ██║██║╚██╔╝██║██║   ██║██║   ██║██║
* ██║██║ ╚═╝ ██║╚██████╔╝╚██████╔╝██║
* ╚═╝╚═╝     ╚═╝ ╚═════╝  ╚═════╝ ╚═╝
*/

char inputBuffer[128] { "methane" };
int cid {1};
float offsetBuffer[3] { 0.f, 4.f, 0.f };
int atomSphereCount {0};
int bondCylinderCount {0};
float rotationSpeed {.1f};
float scalar {4.f};
bool particlesEnabled {true};

struct ChemistryQueueResult {
    std::vector<glm::mat4> matricesAtoms;
    std::vector<glm::mat4> matricesBonds;
    int atomsCount;
    int bondsCount;
};

std::queue<ChemistryQueueResult> chemistryQueue;

void Renderer::imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Properties");

    if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen)) { 
        ImGui::InputFloat("Sensitivity", &camera->sens);
        ImGui::InputFloat("Speed", &camera->speed);
    }

    if (ImGui::CollapsingHeader("Molecule Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Compound", inputBuffer, sizeof(inputBuffer));
        if (ImGui::Button("Summon by name")) {    
            auto doMatrixCalculations = [=] {
                Chemistry::Molecule molecule = Chemistry::loadMolecule(inputBuffer);
                int instanceCountAtoms = molecule.atoms.size();
                if (instanceCountAtoms > 0) {
                    atomSphereCount = instanceCountAtoms;
                    bondCylinderCount = molecule.bondsCount;
                    glm::vec3 offset = glm::vec3(offsetBuffer[0], offsetBuffer[1], offsetBuffer[2]);
                    auto instanceMatricesAtoms = Chemistry::createAtomMatrices(molecule, scalar);
                    auto instanceMatricesBonds = Chemistry::createBondMatrices(molecule, scalar);    
                    chemistryQueue.push(ChemistryQueueResult {
                        instanceMatricesAtoms,
                        instanceMatricesBonds,
                        instanceCountAtoms,
                        bondCylinderCount
                    });
                }    
            };
            std::thread matrixCalculationsThread(doMatrixCalculations);
            matrixCalculationsThread.detach(); 
        }

        ImGui::InputInt("CID", &cid);
        if (ImGui::Button("Summon by CID")) {    
            auto doMatrixCalculations = [=] {
                Chemistry::Molecule molecule = Chemistry::loadMolecule(cid);
                int instanceCountAtoms = molecule.atoms.size();
                if (instanceCountAtoms > 0) {
                    atomSphereCount = instanceCountAtoms;
                    bondCylinderCount = molecule.bondsCount;
                    glm::vec3 offset = glm::vec3(offsetBuffer[0], offsetBuffer[1], offsetBuffer[2]);
                    auto instanceMatricesAtoms = Chemistry::createAtomMatrices(molecule, scalar);
                    auto instanceMatricesBonds = Chemistry::createBondMatrices(molecule, scalar);    
                    chemistryQueue.push(ChemistryQueueResult {
                        instanceMatricesAtoms,
                        instanceMatricesBonds,
                        instanceCountAtoms,
                        bondCylinderCount
                    });
                }
            };
            std::thread matrixCalculationsThread(doMatrixCalculations);
            matrixCalculationsThread.detach(); 
        }
        
        ImGui::InputFloat("Scalar", &scalar);
        ImGui::InputFloat3("Offset", offsetBuffer);
        ImGui::InputFloat("Rotation", &rotationSpeed);
    }

    if (ImGui::CollapsingHeader("Particle Settings", 0)) { 
        ImGui::Checkbox("enabled", &particlesEnabled);
    }


    if (chemistryQueue.size() > 0) {
        auto& chemistryQueueResult = chemistryQueue.front();

        atomSphereObj->getVAO()->bind();
        std::unique_ptr<VBO> instanceVBO = std::make_unique<VBO>();
        instanceVBO->bind();
        instanceVBO->data(&chemistryQueueResult.matricesAtoms.data()[0], chemistryQueueResult.atomsCount * sizeof(glm::mat4), GL_STATIC_DRAW);
        atomSphereObj->getVAO()->attribInstanced(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        atomSphereObj->getVAO()->attribInstanced(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        atomSphereObj->getVAO()->attribInstanced(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        atomSphereObj->getVAO()->attribInstanced(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
        instanceVBO->unbind();
        atomSphereObj->getVAO()->unbind();
    
        bondCylinderObj->getVAO()->bind();
        std::unique_ptr<VBO> instanceBondsVBO = std::make_unique<VBO>();
        instanceBondsVBO->bind();
        instanceBondsVBO->data(&chemistryQueueResult.matricesBonds.data()[0], chemistryQueueResult.bondsCount * sizeof(glm::mat4), GL_STATIC_DRAW);
        bondCylinderObj->getVAO()->attribInstanced(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        bondCylinderObj->getVAO()->attribInstanced(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        bondCylinderObj->getVAO()->attribInstanced(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        bondCylinderObj->getVAO()->attribInstanced(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
        instanceBondsVBO->unbind();
        bondCylinderObj->getVAO()->unbind();
    
        instanceVBO->destroy();
        instanceBondsVBO->destroy();

        chemistryQueue.pop();
    }

    ImGui::End();
}

/**
* ██████╗ ███████╗███╗   ██╗██████╗ ███████╗██████╗ ██╗███╗   ██╗ ██████╗ 
* ██╔══██╗██╔════╝████╗  ██║██╔══██╗██╔════╝██╔══██╗██║████╗  ██║██╔════╝ 
* ██████╔╝█████╗  ██╔██╗ ██║██║  ██║█████╗  ██████╔╝██║██╔██╗ ██║██║  ███╗
* ██╔══██╗██╔══╝  ██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗██║██║╚██╗██║██║   ██║
* ██║  ██║███████╗██║ ╚████║██████╔╝███████╗██║  ██║██║██║ ╚████║╚██████╔╝
* ╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ 
*/
void Renderer::render(float width, float height, float deltaTime) {
    fbo->bind(GL_FRAMEBUFFER);
    glEnable(GL_DEPTH_TEST);

    glClearColor(1.f, .0f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDepthFunc(GL_LESS);

    shaders["defaultShader"]
        ->use()
        ->SetMatrix4x4("projection", camera->projection)
        ->SetMatrix4x4("view", camera->matrix)
        ->unuse();

    objects["quadObject"]->render([this] {
        textures["checkered"]->bind(GL_TEXTURE0);
        shaders["defaultShader"]
            ->use()
            ->SetMatrix4x4("model", objects["quadObject"]
            ->matrix)
            ->SetVector2("texTiling", glm::vec2(20))
            ->SetVector3("texTint", glm::vec3(1))
            ->SetFloat("metallic", 1.f)
            ->SetVector3("cameraPos", this->camera->position)
            ->SetInt("tex", 0)
            ->SetInt("cubemap", 2);
    });
    shaders["defaultShader"]->unuse();

    // objects["sphereObject"]->render([]() {  
    //     shaders["defaultShader"]
    //         ->use()
    //         ->SetInt("tex", 0)
    //         ->SetInt("normalMap", 1)
    //         ->SetFloat("metallic", 0.f)
    //         ->SetVector2("texTiling", glm::vec2(1))
    //         ->SetVector3("texTint", glm::vec3(0, 0, .8))
    //         ->SetMatrix4x4("model", objects["sphereObject"]->matrix);
    // });
    // // objects["sphereObject"]->getComponent<ComponentSomething>()->doSomething();
    // textures["checkered"]->unbind();
    // shaders["defaultShader"]->unuse();

    if (atomSphereObj && bondCylinderObj) {
        shaders["instancingShader"]
            ->use()
            ->SetMatrix4x4("projection", camera->projection)
            ->SetMatrix4x4("view", camera->matrix)
            ->SetFloat("time", glfwGetTime())
            ->SetFloat("rotationSpeed", rotationSpeed)
            ->SetVector3("offset", glm::vec3(offsetBuffer[0], offsetBuffer[1], offsetBuffer[2]));

        atomSphereObj->renderInstanced(atomSphereCount);
        bondCylinderObj->renderInstanced(bondCylinderCount);

        shaders["instancingShader"]->unuse();
    }

    if (particlesEnabled) {
        shaders["particleShader"]
        ->use()
        ->SetMatrix4x4("projection", camera->projection)
        ->SetMatrix4x4("view", camera->matrix);
        vao->bind();
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particleSystem.points.size() / 3));
        vao->unbind();
        shaders["particleShader"]
            ->unuse();
    }

    glDepthFunc(GL_LEQUAL);
    objects["cubemapObject"]->render([this] {
        textures["cubemap"]->bind(GL_TEXTURE2);
        shaders["cubemapShader"]
            ->use()
            ->SetMatrix4x4("projection", this->camera->projection)
            ->SetMatrix4x4("view", glm::mat4(glm::mat3(this->camera->matrix)))
            ->SetInt("cubemap", 2);    
    });

    
    /**
    * ███████╗██████╗  ██████╗ 
    * ██╔════╝██╔══██╗██╔═══██╗
    * █████╗  ██████╔╝██║   ██║
    * ██╔══╝  ██╔══██╗██║   ██║
    * ██║     ██████╔╝╚██████╔╝
    * ╚═╝     ╚═════╝  ╚═════╝ 
    */

    fbo->bind(GL_READ_FRAMEBUFFER);
    intermediateFBO->bind(GL_DRAW_FRAMEBUFFER);
    
    fbo->blit(camera, GL_COLOR_ATTACHMENT0);
    fbo->blit(camera, GL_COLOR_ATTACHMENT1);
    fbo->blit(camera, GL_COLOR_ATTACHMENT2);
    fbo->blit(camera, GL_COLOR_ATTACHMENT3);
    
    glBlitFramebuffer(0, 0, camera->width, camera->height, 0, 0, camera->width, camera->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    
    fbo->unbind();

    glDisable(GL_DEPTH_TEST);

    glClearColor(1.f, .0f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    intermediateFBO->gAlbedo->bind(GL_TEXTURE0);
    intermediateFBO->gNormal->bind(GL_TEXTURE1);
    intermediateFBO->gPosition->bind(GL_TEXTURE2);
    intermediateFBO->gDepth->bind(GL_TEXTURE3);
    intermediateFBO->gMaterial->bind(GL_TEXTURE4);
    

    objects["quadObject"]->render([this]() {
        shaders["screenShader"]
            ->use()
            ->SetInt("albedoTex", 0)
            ->SetInt("positionTex", 2)
            ->SetInt("normalTex", 1)
            ->SetInt("depthTex", 3)
            ->SetInt("materialTex", 4)
            ->SetInt("gBufferTex", this->gBufferIndex)
            ->SetMatrix4x4("projection", camera->projection)
            ->SetMatrix4x4("view", camera->matrix)
            ->SetVector3("cameraPos", camera->position)
            ->SetFloat("time", glfwGetTime());
    });

    intermediateFBO->gAlbedo->unbind();
    intermediateFBO->gDepth->unbind();
    
    fontRenderer->render(std::to_string(int(1.0f/deltaTime)), camera->ortho, glm::vec2(10, height - 280 + 22));
    fontRenderer->render(gBufferTextures[gBufferIndex], camera->ortho, glm::vec2(10, height - 280));

    textures["checkered"]->bind();
}

void Renderer::recompileShaders() {
    for (auto& [_, shader] : shaders){
        shader->recompile();
    }
}

void Renderer::refactor(float width, float height) {
    camera->refactor(width, height);
    fbo->refactor(width, height);
    intermediateFBO->refactor(width, height);
}

/**
*  ██████╗██╗     ███████╗ █████╗ ███╗   ██╗██╗   ██╗██████╗ 
* ██╔════╝██║     ██╔════╝██╔══██╗████╗  ██║██║   ██║██╔══██╗
* ██║     ██║     █████╗  ███████║██╔██╗ ██║██║   ██║██████╔╝
* ██║     ██║     ██╔══╝  ██╔══██║██║╚██╗██║██║   ██║██╔═══╝ 
* ╚██████╗███████╗███████╗██║  ██║██║ ╚████║╚██████╔╝██║     
*  ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝     
*/
void Renderer::cleanup() {
    for (auto& [_, object] : objects) {
        object->destroy();
    }
    
    for (auto& [_, shader] : shaders){
        shader->destroy();
    }

    for (auto& [_, texture] : textures) {
        texture->destroy();
    }

    fbo->destroy();
    intermediateFBO->destroy();
}