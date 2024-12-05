#pragma once
#include "../pch.h"
#include "VulkanContext.hpp"

struct DescriptorSetLayout
{
	VkDescriptorSetLayout _layout;
	uint32_t _binding;

	void createUniformBufferLayout(std::shared_ptr<Context> context, uint32_t binding);
	void createTextureLayout(std::shared_ptr<Context> context, uint32_t binding);
	
	void destroy(std::shared_ptr<Context> context);
};
