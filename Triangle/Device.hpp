#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <cstring>

#include "pch.h"
#include "vulkan/vulkan.h"
#include "Window.hpp"


class Device
{
private:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		
		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

public:
	Device(std::shared_ptr<Window> window);

	Device(Device&) = delete;
	Device(Device&&) = delete;
	void operator=(Device&) = delete;
	void operator=(Device&&) = delete;

	void init();
	void destroy();

private:
	std::shared_ptr<Window> _window;

	VkDevice _device;
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _physicalDevice;
	VkSurfaceKHR _surface;

	VkQueue _graphicsQueue;
	VkQueue _presentQueue;

	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void pickPhysicalDevice();
	int rateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
	void createSurface();

	VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);


	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

};
