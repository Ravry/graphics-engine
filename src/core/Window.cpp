#include "Window.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width = width;
        win->height = height;
        win->renderer->refactor((float)width, (float)height);
    }
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 
    std::cout << "::ERROR:: " << message <<  std::endl;
}

bool cursorDisabled = true;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
            return;
        }        

        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            {
                switch (key)
                {
                    case GLFW_KEY_K: {
                        win->renderer->gBufferIndex = (win->renderer->gBufferIndex + 1) % 5;
                        break;
                    }
                    case GLFW_KEY_C: {    
                        cursorDisabled = !cursorDisabled;
                        glfwSetInputMode(window, GLFW_CURSOR, cursorDisabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                        break;
                    }
                    case GLFW_KEY_R: {
                        win->renderer->recompileShaders();
                        break;
                    }
                }
            }
        }
    }
}

Window::Window(const char* title, const int width, const int height) : width(width), height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::run() {
    renderer = std::make_unique<Renderer>(width, height);
    input = std::make_unique<Input>();

    float deltaTime = 0;
    float lastFrame = 0;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = (currentFrame - lastFrame);
        lastFrame = currentFrame;

        glfwPollEvents();
        input->update(window);
        renderer->update(*input, deltaTime);

        renderer->render(width, height, deltaTime);

        glfwSwapBuffers(window);
    }

    renderer->cleanup();
    destroy();
}

void Window::destroy() {
    glfwTerminate();
}

void Window::set_icon(const char* filename) {
    GLFWimage images[1];
    images[0].pixels = stbi_load(filename, &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);
}