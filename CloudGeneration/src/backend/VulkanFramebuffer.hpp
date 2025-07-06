#pragma once
#include "../pch.h"
#include "vulkan/vulkan.h"
#include "VulkanSwapchain.hpp"
#include "VulkanDevice.hpp"

class Framebuffer
{
public:
	void create(std::shared_ptr<Context> context, Swapchain& swapchain, VkRenderPass renderPass);
	void destroy(std::shared_ptr<Context> context);

	void recreate(std::shared_ptr<Context> context, Swapchain& swapchain, VkRenderPass renderPass);

	VkFramebuffer getFramebuffer(uint32_t index);

private:
	std::vector<VkFramebuffer> _swapChainFramebuffers;

};