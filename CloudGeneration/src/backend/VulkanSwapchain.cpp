#include "VulkanSwapchain.hpp"

void Swapchain::create(std::shared_ptr<Context> context, VkRenderPass renderPass, uint32_t width, uint32_t height) {
	Device::SwapChainSupportDetails swapChainSupport = context->querySwapChainSupport();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = context->getInstance().getSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	Device::QueueFamilyIndices indices = context->getDevice().getQueueFamilyIndices();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(context->getDevice().getDevice(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
		LOG_ERROR("VulkanSwapchain, failed to create swap chain !");
		throw std::runtime_error("Error: Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(context->getDevice().getDevice(), _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(context->getDevice().getDevice(), _swapChain, &imageCount, _swapChainImages.data());
	createImageViews(context->getDevice());
	createDepthResources(context);
}

void Swapchain::recreate(std::shared_ptr<Context> context, VkRenderPass renderPass, uint32_t width, uint32_t height)
{
	destroy(context);
	create(context, renderPass, width, height);
}

void Swapchain::destroy(std::shared_ptr<Context> context)
{
	_depthResource.destroy(context);

	for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
		vkDestroyImageView(context->getDevice().getDevice(), _swapChainImageViews[i], nullptr);
	}
	vkDestroySwapchainKHR(context->getDevice().getDevice(), _swapChain, nullptr);
}

VkSwapchainKHR Swapchain::get()
{
	return _swapChain;
}

VkFormat Swapchain::getFormat()
{
	return _swapChainImageFormat;
}

uint32_t Swapchain::getSize()
{
	return _swapChainImageViews.size();
}

VkExtent2D Swapchain::getExtent()
{
	return _swapChainExtent;
}

uint32_t Swapchain::getWidth()
{
	return _swapChainExtent.width;
}

uint32_t Swapchain::getHeight()
{
	return _swapChainExtent.height;
}

VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
{
	if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
		return capabilities.currentExtent;
	}
	else {

		VkExtent2D actualExtent {width,height};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void Swapchain::createImageViews(Device& device)
{
	_swapChainImageViews.resize(_swapChainImages.size());

	for (size_t i = 0; i < _swapChainImages.size(); i++) {
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _swapChainImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = _swapChainImageFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		if (vkCreateImageView(device.getDevice(), &viewInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
			LOG_ERROR("VulkanSwapchain, failed to create texture image view !");
			throw std::runtime_error("Error : failed to create texture image view!");
		}
	}
}

void Swapchain::createDepthResources(std::shared_ptr<Context> context)
{
	VkFormat depthFormat = context->findDepthFormat();

	_depthResource.createImage(context, getWidth(), getHeight(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_depthResource.createImageView(context, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	_depthResource.transitionImageLayout(context, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

bool Swapchain::hasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}
