#include "VulkanDevice.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

Device::Device() :
	_device(VK_NULL_HANDLE),
	_physicalDevice(VK_NULL_HANDLE)
{
}

void Device::init(VkInstance instance, VkSurfaceKHR surface)
{
	pickPhysicalDevice(instance, surface);
	createLogicalDevice(surface);
}

void Device::destroy()
{
	vkDestroyDevice(_device, nullptr);
}

void Device::waitDeviceIdle()
{
	vkDeviceWaitIdle(_device);
}

VkDevice Device::getDevice()
{
	return _device;
}

VkPhysicalDevice Device::getPhysicalDevice()
{
	return _physicalDevice;
}

Device::QueueFamilyIndices Device::getQueueFamilyIndices()
{
	return _queueFamilyIndices;
}

bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool Device::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	auto extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void Device::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		LOG_ERROR("VulkanDevice, failed to find GPUs with Vulkan support !");
		throw std::runtime_error("Error: Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device, surface)) {
			_physicalDevice = device;
			break;
		}
	}

	if (_physicalDevice == VK_NULL_HANDLE) {
		LOG_ERROR("VulkanDevice, failed to find a suitable GPU !");
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void Device::createLogicalDevice(VkSurfaceKHR surface)
{
	_queueFamilyIndices = findQueueFamilies(_physicalDevice, surface);
	float queuePriority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { _queueFamilyIndices.graphicsFamily.value(), _queueFamilyIndices.presentFamily.value() };

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
		LOG_ERROR("VulkanDevice, failed to create logical device !");
		throw std::runtime_error("Error: Failed to create logical device!");
	}
}

Device::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices{};
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}
		i++;
	}
	return indices;
}

Device::SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	Device::SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Check Format Mode
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// Check Present Mode
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}