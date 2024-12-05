#include "VulkanContext.hpp"

void Context::create(std::shared_ptr<Window> window)
{
	_instance.createInstance(window);
	_device.init(_instance.getInstance(), _instance.getSurface());
	createQueues();
	createCommandPool();
}

void Context::destroy()
{

	_device.destroy();
	_instance.destroyInstance();
}

Device& Context::getDevice()
{
	return _device;
}

Instance& Context::getInstance()
{
	return _instance;
}

VkCommandPool Context::getCommandPool()
{
	return _commandPool;
}

VkQueue Context::getGraphicsQueue()
{
	return _graphicsQueue;
}

VkQueue Context::getPresentQueue()
{
	return _presentQueue;
}

void Context::waitDeviceIdle()
{
	_device.waitDeviceIdle();
}

VkCommandBuffer Context::beginSingleTimeCommands() {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_device.getDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Context::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_graphicsQueue); // Can be replaced by fence for opti with multiple buffers

	vkFreeCommandBuffers(_device.getDevice(), _commandPool, 1, &commandBuffer);
}

void Context::createQueues()
{
	Device::QueueFamilyIndices indices = _device.getQueueFamilyIndices();

	vkGetDeviceQueue(_device.getDevice(), indices.graphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_device.getDevice(), indices.presentFamily.value(), 0, &_presentQueue);
}

void Context::createCommandPool()
{
	Device::QueueFamilyIndices indices = _device.getQueueFamilyIndices();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

	if (vkCreateCommandPool(_device.getDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create command pool!");
	}
}

Device::SwapChainSupportDetails Context::querySwapChainSupport() {
	return _device.querySwapChainSupport(_device.getPhysicalDevice(), _instance.getSurface());
}

uint32_t Context::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_device.getPhysicalDevice(), &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Error : failed to find suitable memory type!");
}

VkFormat Context::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_device.getPhysicalDevice(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	throw std::runtime_error("Error : failed to find supported format!");
}

VkFormat Context::findDepthFormat()
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}
