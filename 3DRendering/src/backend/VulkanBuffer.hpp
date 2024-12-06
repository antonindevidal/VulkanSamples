#pragma once
#include "../pch.h"
#include <vulkan/vulkan.h>
#include "VulkanContext.hpp"

struct Buffer
{
	VkBuffer _buffer;
	VkDeviceMemory _bufferMemory;

	void create(std::shared_ptr<Context> context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	void destroy(std::shared_ptr<Context> context);
	void copyTo(std::shared_ptr<Context> context, VkBuffer dstBuffer, VkDeviceSize size);
	void copyToImage(std::shared_ptr<Context> context, VkImage image, uint32_t width, uint32_t height);
};
