#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <cstring>

#include "pch.h"
#include "Helpers.hpp"
#include "vulkan/vulkan.h"
#include "Window.hpp"

class Device
{
public:
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

public:
	Device();

	void init(VkInstance instance, VkSurfaceKHR surface);
	void destroy();

	void waitDeviceIdle();

	VkDevice getDevice();
	VkPhysicalDevice getPhysicalDevice();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

private:


	VkDevice _device;
	VkPhysicalDevice _physicalDevice;
	

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
	//int rateDeviceSuitability(VkPhysicalDevice device);
	void createLogicalDevice(VkSurfaceKHR surface);




};
