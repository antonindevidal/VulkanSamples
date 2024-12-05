#pragma once
#include "../pch.h"
#include "../Helpers.hpp"
#include "VulkanContext.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTexture.hpp"

class Swapchain
{
public:
	// Swapchain creation
	void create(std::shared_ptr<Context> context, VkRenderPass renderPass, uint32_t width, uint32_t height);
	void recreate(std::shared_ptr<Context> context, VkRenderPass renderPass, uint32_t width, uint32_t height);
	void destroy(std::shared_ptr<Context> context);

	VkSwapchainKHR get();
	VkFormat getFormat();
	uint32_t getSize();
	VkExtent2D getExtent();

	uint32_t getWidth();
	uint32_t getHeight();

private:
	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	std::vector<VkImageView> _swapChainImageViews;

	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;

	Texture _depthResource;

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
	void createImageViews(Device& device);
	void createDepthResources(std::shared_ptr<Context> context);

	bool hasStencilComponent(VkFormat format);
	
	
	friend class Framebuffer;
};