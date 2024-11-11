#include "Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Window.hpp"
#include "Device.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

int main() {

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Device device{ window };
    device.init();

    Mesh mesh;
    mesh.createMesh(device, vertices, indices);

    

    while (!window->ShouldClose()) {
        window->PollEvents();
        device.startFrame();

        mesh.drawMesh(device);
        device.endFrame();
    }
    device.waitDeviceIdle();

    mesh.destroyMesh(device);

    device.destroy();
    window->Destroy();
    return 0;
}