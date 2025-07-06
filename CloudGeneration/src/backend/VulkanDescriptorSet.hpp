#pragma once
#include "../pch.h"
#include "VulkanContext.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanDescriptorSetLayout.hpp"
#include "VulkanDescriptorPool.hpp"
#include "VulkanTexture.hpp"
#include "VulkanUniformBuffer.hpp"

struct DescriptorSet
{
	VkDescriptorSetLayout _descriptorSetLayout;
	std::vector<VkDescriptorSet> _descriptorSets;

	void createDescriptorSetTexture(std::shared_ptr<Context> context, DescriptorSetLayout layout, DescriptorPool pool, Texture t);
	void createDescriptorSetUniformBuffer(std::shared_ptr<Context> context, DescriptorSetLayout layout, DescriptorPool pool, UniformBuffer ub);
	void destroy(std::shared_ptr<Context> context);
	
	void bind(std::shared_ptr<Context> context, GraphicsPipeline gp, VkCommandBuffer commandBuffer, uint32_t currentFrame, uint32_t layoutSetIndex = 0u) const;
};