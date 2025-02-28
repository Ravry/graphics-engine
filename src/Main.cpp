#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include "headers/Window.hpp"
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    std::unique_ptr<Window> window = std::make_unique<Window>("", SCR_WIDTH, SCR_HEIGHT);
    window->set_icon("resources/texture/checkered.png");
    window->run();
    return 0;
}