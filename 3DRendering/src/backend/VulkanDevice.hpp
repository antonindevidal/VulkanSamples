#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "../pch.h"
#include "../Helpers.hpp"
#include "../Window.hpp"

class Device
{
public:
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

public:
	Device();

	void init(VkInstance instance, VkSurfaceKHR surface);
	void destroy();

	void waitDeviceIdle();

	VkDevice getDevice();
	VkPhysicalDevice getPhysicalDevice();
	QueueFamilyIndices getQueueFamilyIndices();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	VkDevice _device;
	VkPhysicalDevice _physicalDevice;
	QueueFamilyIndices _queueFamilyIndices;

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
	void createLogicalDevice(VkSurfaceKHR surface);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
};
