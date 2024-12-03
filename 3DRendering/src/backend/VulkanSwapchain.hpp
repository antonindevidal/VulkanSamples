#pragma once
#include "../pch.h"
#include "../Helpers.hpp"
#include "../Device.hpp"
#include "VulkanContext.hpp"

class Swapchain
{


public:
	// Swapchain creation
	void create(std::shared_ptr<Context> context, VkRenderPass renderPass, uint32_t width, uint32_t height);
	void recreate(std::shared_ptr<Context> context, VkRenderPass renderPass, uint32_t width, uint32_t height);
	void cleanup(Device& device);

	VkSwapchainKHR get();
	VkFormat getFormat();
	uint32_t getSize();
	VkExtent2D getExtent();

	uint32_t getWidth();
	uint32_t getHeight();

private:
	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;

	VkImage _depthImage;
	VkDeviceMemory _depthImageMemory;
	VkImageView _depthImageView;

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
	void createImageViews(Device& device);
	void createDepthResources(std::shared_ptr<Context> context);

	// TODO : move to texture class
	void createImage(Device& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView createImageView(Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void transitionImageLayout(std::shared_ptr<Context> context, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	bool hasStencilComponent(VkFormat format);
	

	// TODO : move to context
	VkFormat findSupportedFormat(Device& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat(Device& device);
	uint32_t findMemoryType(Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	
	friend class Framebuffer;
};