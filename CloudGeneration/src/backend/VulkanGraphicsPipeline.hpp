#pragma once
#include "../pch.h"
#include "VulkanContext.hpp"
#include "VulkanVertexBuffer.hpp"

struct GraphicsPipeline
{
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	void create(std::shared_ptr<Context> context, uint32_t width, uint32_t height, VkExtent2D extent, VkRenderPass renderPass,  const std::string& vertexShaderPath, const std::string& fragmentShaderPath, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
	void createRaymarchingGraphicsPipeline(std::shared_ptr<Context> context, uint32_t width, uint32_t height, VkExtent2D extent, VkRenderPass renderPass, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);

	void destroy(std::shared_ptr<Context> context);
	void bind(std::shared_ptr<Context> context, VkCommandBuffer commandBuffer);
	VkShaderModule createShaderModule(std::shared_ptr<Context> context, const std::vector<char>& code);

};
