#include "Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Window.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

int main() {

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Renderer renderer{};
    renderer.createRenderer(window);

    Mesh mesh = renderer.createMesh(vertices, indices);
    

    while (!window->ShouldClose()) {
        window->PollEvents();
        renderer.startFrame();

        renderer.drawMesh(mesh);

        renderer.endFrame();
    }
    renderer.waitDeviceIdle();

    renderer.destroyMesh(mesh);

    renderer.destroy();
    window->Destroy();
    return 0;
}