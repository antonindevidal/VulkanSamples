#pragma once
#include "pch.h"

struct Buffer
{
	VkBuffer _buffer;
	VkDeviceMemory _bufferMemory;
};

struct UniformBuffer
{
	std::vector<VkBuffer> _buffers;
	std::vector<VkDeviceMemory> _buffersMemory;
	std::vector<void*> _buffersMapped;
	uint32_t _size;

	VkDescriptorBufferInfo descriptorInfo(int index)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _buffers[index];
		bufferInfo.offset = 0;
		bufferInfo.range = _size;
		return bufferInfo;
	}
};
