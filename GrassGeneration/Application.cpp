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
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count() *0.5;

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);
    ubo.proj[1][1] *= -1;

    return ubo;
}

int main() {

    // Create grassBlade data
    uint32_t nbBlades = 1000000;



    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Renderer renderer{};
    renderer.createRenderer(window);

    Mesh meshGround = renderer.createMesh(verticesGround, indicesGround);
    Mesh meshGrass= renderer.createMesh("Models/grass2.obj");

    Texture texture = renderer.createTexture("Textures/grass.png");
    
    UniformBuffer uniforms = renderer.createUniformBuffer<UniformBufferObject>();
    //ShaderStorageBufferObject ssboGrass = renderer.createShaderStorageBuffer(grassData);
    ShaderStorageBufferObject ssboGrass = renderer.createShaderStorageBuffer(sizeof(GrassBladeData),nbBlades);

    DescriptorPool pool = renderer.createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,2}, {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2 } }, 10);

    DescriptorSetLayout layoutUb = renderer.createDescriptorSetLayoutBuffer();
    DescriptorSetLayout layoutText = renderer.createDescriptorSetlayout(texture);
    DescriptorSetLayout layoutGrassSSBO = renderer.createDescriptorSetLayoutGrass();
    DescriptorSetLayout layoutGrassSSBOCompute = renderer.createDescriptorSetLayoutGrassCompute();

    DescriptorSet descriptorSetUb = renderer.createDescriptorSet(layoutUb, pool, uniforms);
    DescriptorSet descriptorSetText = renderer.createDescriptorSet(layoutText, pool, texture);
    DescriptorSet descriptorSetGrass = renderer.createDescriptorSetGrass(layoutGrassSSBO, pool, ssboGrass, texture);
    DescriptorSet descriptorSetGrassCompute = renderer.createDescriptorSetGrassCompute(layoutGrassSSBOCompute, pool, ssboGrass);

    GraphicsPipeline pipeline = renderer.createGraphicsPipeline("Shaders/vert.spv","Shaders/colorfrag.spv", { layoutUb,layoutText });
    GraphicsPipeline pipelineGrass = renderer.createGraphicsPipeline("Shaders/grassvert.spv","Shaders/frag.spv", { layoutUb, layoutGrassSSBO });

    ComputePipeline pipelineGrassGen = renderer.createComputePipeline("Shaders/grassGenComp.spv", { layoutGrassSSBOCompute });

    
    renderer.startComputeRecoring();
    renderer.bindComputePipeline(pipelineGrassGen);
    renderer.bindDescriptorSet(descriptorSetGrassCompute, 0, VK_PIPELINE_BIND_POINT_COMPUTE);
    renderer.endComputeRecoring(nbBlades, 1, 1);


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
        renderer.drawMesh(meshGround);

        renderer.bindGraphicsPipeline(pipelineGrass);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.bindDescriptorSet(descriptorSetGrass, 1);
        renderer.drawMesh(meshGrass, nbBlades);



        renderer.endFrame();
    }
    renderer.waitDeviceIdle();

    renderer.destroyDescriptorPool(pool);
    renderer.destroyDescriptorSetLayout(layoutText);
    renderer.destroyDescriptorSetLayout(layoutUb);
    renderer.destroyDescriptorSetLayout(layoutGrassSSBO);
    renderer.destroyDescriptorSetLayout(layoutGrassSSBOCompute);

    renderer.destroyComputePipeline(pipelineGrassGen);
    renderer.destroyGraphicsPipeline(pipeline);
    renderer.destroyGraphicsPipeline(pipelineGrass);

    renderer.destroyUniformBuffer(uniforms);
    renderer.destroyShaderStorageBufferObject(ssboGrass);
    renderer.destroyTexture(texture);
    renderer.destroyMesh(meshGround);
    renderer.destroyMesh(meshGrass);

    renderer.destroy();
    window->Destroy();
    return 0;
}