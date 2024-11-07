#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <cstring>

#include "pch.h"
#include "Helpers.hpp"
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

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

public:
	Device(std::shared_ptr<Window> window);

	Device(Device&) = delete;
	Device(Device&&) = delete;
	void operator=(Device&) = delete;
	void operator=(Device&&) = delete;

	void init();
	void destroy();

	void drawFrame();
	void waitDeviceIdle();
private:
	const int MAX_FRAMES_IN_FLIGHT = 2;

	std::shared_ptr<Window> _window;

	VkDevice _device;
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _physicalDevice;
	VkSurfaceKHR _surface;
	
	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	VkRenderPass _renderPass;
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
	std::vector<VkFramebuffer> _swapChainFramebuffers;

	VkCommandPool _commandPool;
	std::vector<VkCommandBuffer> _commandBuffers;
	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;

	bool _framebufferResized = false;

	VkQueue _graphicsQueue;
	VkQueue _presentQueue;

	uint32_t _currentFrame = 0;

	void createInstance();
	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void pickPhysicalDevice();
	//int rateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
	void createSurface();
	void createImageViews();
	void createGraphicsPipeline();
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	// Swapchain creation
	void createSwapChain();
	void recreateSwapChain();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void cleanupSwapChain();

	VkShaderModule createShaderModule(const std::vector<char>& code);


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

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

};
