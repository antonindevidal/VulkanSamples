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
	QueueFamilyIndices indices = findQueueFamilies(_device.getPhysicalDevice(), _instance.getSurface());

	vkGetDeviceQueue(_device.getDevice(), indices.graphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_device.getDevice(), indices.presentFamily.value(), 0, &_presentQueue);
}

void Context::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_device.getPhysicalDevice(), _instance.getSurface());

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(_device.getDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create command pool!");
	}
}

Device::SwapChainSupportDetails Context::querySwapChainSupport() {
	return _device.querySwapChainSupport(_device.getPhysicalDevice(), _instance.getSurface());
}
