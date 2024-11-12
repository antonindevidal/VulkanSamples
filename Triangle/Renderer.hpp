#pragma once
#include "pch.h"
#include "Device.hpp"
#include "Mesh.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

class Renderer
{
private:
	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};


public:
	Renderer();

	void createRenderer(std::shared_ptr<Window> window);
	void destroy();

	void startFrame();
	void endFrame();

	VkCommandBuffer getCommandBuffer();
	VkPipelineLayout getPipelineLayout();
	VkDescriptorSet& getDescriptorSet();
	Device& getDevice();
	void waitDeviceIdle();

private:
	std::shared_ptr<Window> _window;

	Device _device;

	VkSurfaceKHR _surface;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	VkRenderPass _renderPass;

	VkDescriptorSetLayout _descriptorSetLayout;
	VkDescriptorPool _descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

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
	uint32_t _currentImageIndex = 0;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkImage _depthImage;
	VkDeviceMemory _depthImageMemory;
	VkImageView _depthImageView;
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;

	void createInstance();
	bool checkValidationLayerSupport();


	void createQueues();
	void createSurface();
	void createImageViews();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	VkSampler createTextureSampler();
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void startRecording(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void endRecording(VkCommandBuffer commandBuffer);

	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	// Swapchain creation
	void createSwapChain();
	void recreateSwapChain();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void cleanupSwapChain();

	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createUniformBuffers();
	void createDescriptorPool();
	void updateUniformBuffer(uint32_t currentFrame);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	std::vector<const char*> getRequiredExtensions();

public:
	// Buffers
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void destroyBuffer(Buffer buffer);
	Buffer createVertexBuffer(std::vector<Vertex> vertices);
	Buffer createIndexBuffer(std::vector<index_t> indices);

	// Mesh
	Mesh createMesh(const std::vector<Vertex>& vertices, const std::vector<index_t>& indices);
	void destroyMesh(Mesh& mesh);
	void drawMesh(Mesh& mesh);

	// Textures
	Texture createTexture(const std::string& image);
	void destroyTexture(Texture texture);


	void createDescriptorSets(VkDescriptorImageInfo textureInfo);


private:
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);



	VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);



};