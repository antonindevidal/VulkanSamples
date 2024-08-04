#include "Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Window.hpp"
#include "Device.hpp"

int main() {
    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    std::shared_ptr<Device> device = std::make_shared<Device>(window);
    device->init();

    uint32_t extensionCount = 0;
    
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!window->ShouldClose()) {
        window->PollEvents();
    }

    device->destroy();
    window->Destroy();
    return 0;
}