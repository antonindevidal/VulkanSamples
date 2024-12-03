#pragma once
#include "../pch.h"
#include <vulkan/vulkan.h>

#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"

class Context
{



public:
	void create(std::shared_ptr<Window> window);
	void destroy();
	Device& getDevice();
	Instance& getInstance();
	VkCommandPool getCommandPool();

	VkQueue getGraphicsQueue();
	VkQueue getPresentQueue();

	void waitDeviceIdle();

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	Device::SwapChainSupportDetails querySwapChainSupport();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
	Device _device;
	Instance _instance;
	VkCommandPool _commandPool;

	VkQueue _graphicsQueue;
	VkQueue _presentQueue;

	void createQueues();
	void createCommandPool();
};