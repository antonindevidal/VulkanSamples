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

int main() {
    IndexBuffer indexBuffer;
    VertexBuffer vertexBuffer;

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Device device{ window };
    device.init();

    vertexBuffer.createVertexBuffer(device, vertices);
    indexBuffer.createIndexBuffer(device, indices);

    

    while (!window->ShouldClose()) {
        window->PollEvents();
        device.startFrame();

        VkBuffer vertexBuffers[] = { vertexBuffer.getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(device.getCommandBuffer(), 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(device.getCommandBuffer(), indexBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(device.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, device.getPipelineLayout(), 0, 1, &(device.getDescriptorSet()), 0, nullptr);

        vkCmdDrawIndexed(device.getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        device.endFrame();
    }
    device.waitDeviceIdle();

    vertexBuffer.destroyBuffer(device);
    indexBuffer.destroyBuffer(device);

    device.destroy();
    window->Destroy();
    return 0;
}