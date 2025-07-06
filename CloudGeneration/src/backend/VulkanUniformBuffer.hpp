#pragma once
#include "../pch.h"
#include "VulkanContext.hpp"
#include "VulkanBuffer.hpp"

struct UniformBuffer
{
	std::vector<Buffer> _buffers;
	std::vector<void*> _buffersMapped;
	uint32_t _size;

	void create(std::shared_ptr<Context> context, size_t size);

	template<class T>
	void update(std::shared_ptr<Context> context, T data, uint32_t currentFrame);
	void destroy(std::shared_ptr<Context> context);

	VkDescriptorBufferInfo descriptorInfo(int index)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _buffers[index]._buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = _size;
		return bufferInfo;
	}
};

template<class T>
inline void UniformBuffer::update(std::shared_ptr<Context> context, T data, uint32_t currentFrame)
{
	if (_size != sizeof(T))
	{
		throw std::runtime_error("Error : Size of Uniform Buffer and data not equal ! ");
	}
	memcpy(_buffersMapped[currentFrame], &data, sizeof(T));
}
