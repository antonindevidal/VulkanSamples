#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <cstring>

#include "pch.h"
#include "Helpers.hpp"
#include "vulkan/vulkan.h"
#include "Window.hpp"
#include "VkStructs.hpp"

class Device
{
private:


public:
	Device();

	Device(Device&) = delete;
	Device(Device&&) = delete;
	void operator=(Device&) = delete;
	void operator=(Device&&) = delete;

	void init(VkInstance instance, VkSurfaceKHR surface);
	void destroy();

	void startFrame();
	void endFrame();
	void waitDeviceIdle();

	VkDevice getDevice();
	VkPhysicalDevice getPhysicalDevice();

private:


	VkDevice _device;
	VkPhysicalDevice _physicalDevice;
	

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
	//int rateDeviceSuitability(VkPhysicalDevice device);
	void createLogicalDevice(VkSurfaceKHR surface);




};
