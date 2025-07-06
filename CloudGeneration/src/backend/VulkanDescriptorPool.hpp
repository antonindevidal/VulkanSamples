#pragma once
#include "../pch.h"
#include "VulkanContext.hpp"

struct DescriptorPool
{
	VkDescriptorPool _pool;

	void create(std::shared_ptr<Context> context, std::vector<std::pair<VkDescriptorType, uint32_t>> infos, uint32_t maxSets);
	void destroy(std::shared_ptr<Context> context);
};
