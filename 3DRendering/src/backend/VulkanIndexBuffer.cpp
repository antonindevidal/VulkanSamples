#include "VulkanIndexBuffer.hpp"

void IndexBuffer::create(std::shared_ptr<Context> context, const std::vector<index_t>& indices)
{
	Buffer stagingBuffer;
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	stagingBuffer.create(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(context->getDevice().getDevice(), stagingBuffer._bufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(context->getDevice().getDevice(), stagingBuffer._bufferMemory);

	_buffer.create(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	stagingBuffer.copyTo(context, _buffer._buffer, bufferSize);
	
	stagingBuffer.destroy(context);
}

void IndexBuffer::destroy(std::shared_ptr<Context> context)
{
	_buffer.destroy(context);
}
