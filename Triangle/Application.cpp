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
    Mesh mesh2 = renderer.createMesh(vertices2, indices2);
    Texture texture = renderer.createTexture("Textures/cat.jpg");
    Texture texture2 = renderer.createTexture("Textures/cat2.jpg");
    UniformBuffer uniforms = renderer.createUniformBuffer<UniformBufferObject>();

    VkDescriptorPool pool = renderer.createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,1} }, 2);
    VkDescriptorSetLayout layoutUb = renderer.createDescriptorSetlayout(uniforms);
    VkDescriptorSetLayout layoutText = renderer.createDescriptorSetlayout(texture);

    DescriptorSet descriptorSetUb = renderer.createDescriptorSet(layoutUb, pool, uniforms);
    DescriptorSet descriptorSetText = renderer.createDescriptorSet(layoutText, pool, texture);
    std::vector<VkDescriptorSetLayout> layouts = { layoutUb,layoutText };
    GraphicsPipeline pipeline = renderer.createGraphicsPipeline("Shaders/vert.spv","Shaders/frag.spv", layouts);


    while (!window->ShouldClose()) {
        window->PollEvents();
        
        // Update
        auto matrices = createMatrices(renderer.getSwapchainWidth(), renderer.getSwapchainHeight());
        renderer.updateUniformBuffer<UniformBufferObject>(uniforms, matrices);

        // Draw
        renderer.startFrame();

        renderer.bindGraphicsPipeline(pipeline);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.bindDescriptorSet(descriptorSetText,1);
        renderer.drawMesh(mesh);

        //renderer.bindDescriptorSet(descriptorSet2);
        renderer.drawMesh(mesh2);

        renderer.endFrame();
    }
    renderer.waitDeviceIdle();

    renderer.destroyGraphicsPipeline(pipeline);
    renderer.destroyDescriptorSet(descriptorSetUb);
    renderer.destroyDescriptorSet(descriptorSetText);
    renderer.destroyUniformBuffer(uniforms);
    renderer.destroyTexture(texture);
    renderer.destroyTexture(texture2);
    renderer.destroyMesh(mesh);
    renderer.destroyMesh(mesh2);

    renderer.destroy();
    window->Destroy();
    return 0;
}