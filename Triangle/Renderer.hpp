#pragma once
#include "pch.h"
#include "Device.hpp"
#include "Mesh.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"
#include "DescriptorSet.hpp"
#include "Instance.hpp"

class Renderer
{
private:


public:
	Renderer();

	void createRenderer(std::shared_ptr<Window> window);
	void destroy();

	void startFrame();
	void endFrame();

	VkCommandBuffer getCommandBuffer();
	VkPipelineLayout getPipelineLayout();
	uint32_t getSwapchainWidth();
	uint32_t getSwapchainHeight();

	Device& getDevice();
	void waitDeviceIdle();

private:
	std::shared_ptr<Window> _window;

	Device _device;
	Instance _instance;


	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	VkRenderPass _renderPass;

	VkDescriptorPool _descriptorPool;

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



	VkImage _depthImage;
	VkDeviceMemory _depthImageMemory;
	VkImageView _depthImageView;

	void createQueues();
	void createImageViews();
	VkDescriptorSetLayout createDescriptorSetLayout();
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
	void createDescriptorPool();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);


public:
	// Graphics pipeline
	void createGraphicsPipeline(DescriptorSet descriptorSet);

	// Buffers
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void destroyBuffer(Buffer buffer);
	Buffer createVertexBuffer(std::vector<Vertex> vertices);
	Buffer createIndexBuffer(std::vector<index_t> indices);

	template<typename T>
	UniformBuffer createUniformBuffer();
	void destroyUniformBuffer(UniformBuffer buffer);
	template<typename T>
	void updateUniformBuffer(UniformBuffer buffer, const T& data);

	// Mesh
	Mesh createMesh(const std::vector<Vertex>& vertices, const std::vector<index_t>& indices);
	void destroyMesh(Mesh& mesh);
	void drawMesh(Mesh& mesh);

	// Textures
	Texture createTexture(const std::string& image);
	void destroyTexture(Texture texture);

	// Descriptors
	DescriptorSet createDescriptorSet(UniformBuffer uniformBuffer, Texture texture);
	void destroyDescriptorSet(DescriptorSet descriptorSet);
	void bindDescriptorSet(DescriptorSet descriptorSet);


};

template<typename T>
UniformBuffer Renderer::createUniformBuffer()
{
	UniformBuffer buffer;
	VkDeviceSize bufferSize = sizeof(T);

	buffer._buffers.resize(MAX_FRAMES_IN_FLIGHT);
	buffer._buffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	buffer._buffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
	buffer._size = bufferSize;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer._buffers[i], buffer._buffersMemory[i]);

		vkMapMemory(_device.getDevice(), buffer._buffersMemory[i], 0, bufferSize, 0, &buffer._buffersMapped[i]);
	}

	return buffer;
}

template<typename T>
void Renderer::updateUniformBuffer(UniformBuffer buffer, const T& data)
{
	memcpy(buffer._buffersMapped[_currentFrame], &data, sizeof(T));
}
