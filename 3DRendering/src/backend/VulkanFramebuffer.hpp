#pragma once
#include "../pch.h"
#include "vulkan/vulkan.h"
#include "VulkanSwapchain.hpp"
#include "../Device.hpp"

class Framebuffer
{
public:
	void create(Device& device, Swapchain& swapchain, VkRenderPass renderPass);
	void destroy(Device& device);

	VkFramebuffer getFramebuffer(uint32_t index);

private:
	std::vector<VkFramebuffer> _swapChainFramebuffers;

};