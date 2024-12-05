#pragma once
#include "../pch.h"

using DescriptorPool = VkDescriptorPool;

struct DescriptorSet
{
	VkDescriptorSetLayout _descriptorSetLayout;
	std::vector<VkDescriptorSet> _descriptorSets;
};

