#pragma once
#include "../pch.h"

using DescriptorPool = VkDescriptorPool;
using DescriptorSetLayout = VkDescriptorSetLayout;

struct DescriptorSet
{
	VkDescriptorSetLayout _descriptorSetLayout;
	std::vector<VkDescriptorSet> _descriptorSets;
};

struct GraphicsPipeline
{
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
};

