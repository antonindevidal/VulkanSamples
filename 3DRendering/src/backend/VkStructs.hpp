#pragma once
#include "../pch.h"

using DescriptorPool = VkDescriptorPool;
struct DescriptorSetLayout
{
	VkDescriptorSetLayout _layout;
	uint32_t _binding;
};

struct DescriptorSet
{
	VkDescriptorSetLayout _descriptorSetLayout;
	std::vector<VkDescriptorSet> _descriptorSets;
};

