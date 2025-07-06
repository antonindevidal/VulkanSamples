#include "VulkanUniformBuffer.hpp"

void UniformBuffer::create(std::shared_ptr<Context> context, size_t size)
{
	_buffers.resize(MAX_FRAMES_IN_FLIGHT);
	_buffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
	_size = size;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		_buffers[i].create(context, _size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		vkMapMemory(context->getDevice().getDevice(), _buffers[i]._bufferMemory, 0, _size, 0, &_buffersMapped[i]);
	}

}

void UniformBuffer::destroy(std::shared_ptr<Context> context)
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		_buffers[i].destroy(context);
	}
}
