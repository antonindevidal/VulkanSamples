#include "Mesh.hpp"

void Mesh::createMesh(Device& device, const std::vector<Vertex>& vertices, const std::vector<index_t>& indices)
{
    _vertexBuffer.createVertexBuffer(device, vertices);
    _indexBuffer.createIndexBuffer(device, indices);

    _nbVertices = vertices.size();
    _nbIndices = indices.size();
}

void Mesh::destroyMesh(Device& device)
{
    _vertexBuffer.destroyBuffer(device);
    _indexBuffer.destroyBuffer(device);
}

void Mesh::drawMesh(Device& device)
{
    VkBuffer vertexBuffers[] = { _vertexBuffer.getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(device.getCommandBuffer(), 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(device.getCommandBuffer(), _indexBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(device.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, device.getPipelineLayout(), 0, 1, &(device.getDescriptorSet()), 0, nullptr);

    vkCmdDrawIndexed(device.getCommandBuffer(), _nbIndices, 1, 0, 0, 0);

}