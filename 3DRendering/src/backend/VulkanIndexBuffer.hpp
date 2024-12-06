#pragma once
#include "../pch.h"
#include <vulkan/vulkan.h>
#include "VulkanContext.hpp"
#include "VulkanBuffer.hpp"

using index_t = uint16_t;

struct IndexBuffer
{
	Buffer _buffer;

	void create(std::shared_ptr<Context> context, const std::vector<index_t>& indices);
	void destroy(std::shared_ptr<Context> context);
};