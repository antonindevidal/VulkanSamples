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



UniformBufferObject createMatrices(int width, int height)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    return ubo;
}

int main() {

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Renderer renderer{};
    renderer.createRenderer(window);

    Mesh mesh = renderer.createMesh(vertices, indices);
    Texture texture = renderer.createTexture("Textures/cat.jpg");
    UniformBuffer uniforms = renderer.createUniformBuffer<UniformBufferObject>();

    renderer.createDescriptorSets(uniforms, texture);

    while (!window->ShouldClose()) {
        window->PollEvents();
        renderer.startFrame();

        renderer.drawMesh(mesh);
        auto matrices = createMatrices(renderer.getSwapchainWidth(), renderer.getSwapchainHeight());
        renderer.updateUniformBuffer<UniformBufferObject>(uniforms, matrices);

        renderer.endFrame();
    }
    renderer.waitDeviceIdle();

    renderer.destroyUniformBuffer(uniforms);
    renderer.destroyTexture(texture);
    renderer.destroyMesh(mesh);

    renderer.destroy();
    window->Destroy();
    return 0;
}