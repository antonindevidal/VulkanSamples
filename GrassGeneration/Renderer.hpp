#pragma once
#include "pch.h"
#include "Device.hpp"
#include "Mesh.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"
#include "DescriptorSet.hpp"
#include "Instance.hpp"
#include "GraphicsPipeline.hpp"

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
	uint32_t getSwapchainWidth();
	uint32_t getSwapchainHeight();

	Device& getDevice();
	void waitDeviceIdle();

private:
	std::shared_ptr<Window> _window;

	Device _device;
	Instance _instance;

	GraphicsPipeline _currentGraphicsPipeline;
	ComputePipeline _currentComputePipeline;


	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	VkRenderPass _renderPass;


	std::vector<VkFramebuffer> _swapChainFramebuffers;

	VkCommandPool _commandPool;
	std::vector<VkCommandBuffer> _commandBuffers;
	VkCommandBuffer _computeCommandBuffer;
	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;

	bool _framebufferResized = false;

	VkQueue _graphicsQueue;
	VkQueue _presentQueue;
	VkQueue _computeQueue;

	uint32_t _currentFrame = 0;
	uint32_t _currentImageIndex = 0;


	VkImage _depthImage;
	VkDeviceMemory _depthImageMemory;
	VkImageView _depthImageView;

	void createQueues();
	void createImageViews();
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
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);


public:
	// Graphics pipeline
	GraphicsPipeline createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
	void destroyGraphicsPipeline(GraphicsPipeline graphicsPipeline);
	void bindGraphicsPipeline(GraphicsPipeline graphicsPipeline);

	// Compute pipeline
	ComputePipeline createComputePipeline(const std::string& computeShaderPath, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
	void destroyComputePipeline(ComputePipeline pipeline);
	void bindComputePipeline(ComputePipeline pipeline);
	void startComputeRecoring();
	void endComputeRecoring(uint32_t x=1, uint32_t y=1, uint32_t z=1);

	// Buffers
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void destroyBuffer(Buffer buffer);
	Buffer createVertexBuffer(std::vector<Vertex> vertices);
	Buffer createIndexBuffer(std::vector<index_t> indices);

	ShaderStorageBufferObject createShaderStorageBuffer(size_t objectSize, uint32_t objectCount);
	template<typename T>
	ShaderStorageBufferObject createShaderStorageBuffer(std::vector<T>& data);
	void destroyShaderStorageBufferObject(ShaderStorageBufferObject ssbo);

	template<typename T>
	UniformBuffer createUniformBuffer();
	void destroyUniformBuffer(UniformBuffer buffer);
	template<typename T>
	void updateUniformBuffer(UniformBuffer buffer, const T& data);

	// Mesh
	Mesh createMesh(const std::vector<Vertex>& vertices, const std::vector<index_t>& indices);
	Mesh createMesh(const std::string& objPath);
	void destroyMesh(Mesh& mesh);
	void drawMesh(Mesh& mesh, uint32_t nbInstances = 1 );

	// Textures
	Texture createTexture(const std::string& image);
	void destroyTexture(Texture texture);

	// Descriptors
	
	DescriptorPool createDescriptorPool(std::vector<std::pair<VkDescriptorType,uint32_t>> infos, uint32_t maxSets);
	void destroyDescriptorPool(DescriptorPool pool);

	DescriptorSetLayout createDescriptorSetlayout(Texture t);
	DescriptorSetLayout createDescriptorSetLayoutBuffer(uint32_t bindingIndex = 0);
	DescriptorSetLayout createDescriptorSetLayoutGrass();
	DescriptorSetLayout createDescriptorSetLayoutGrassCompute();

	void destroyDescriptorSetLayout(DescriptorSetLayout layout);

	DescriptorSet createDescriptorSet(DescriptorSetLayout layout, DescriptorPool pool, Texture t);
	DescriptorSet createDescriptorSet(DescriptorSetLayout layout, DescriptorPool pool, UniformBuffer ub);
	DescriptorSet createDescriptorSetGrass(DescriptorSetLayout layout, DescriptorPool pool, ShaderStorageBufferObject ssbo, Texture texture);
	DescriptorSet createDescriptorSetGrassCompute(DescriptorSetLayout layout, DescriptorPool pool, ShaderStorageBufferObject ssbo);

	void destroyDescriptorSet(DescriptorSet descriptorSet);
	void bindDescriptorSet(DescriptorSet descriptorSet, uint32_t index = 0, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);

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

template<typename T>
ShaderStorageBufferObject Renderer::createShaderStorageBuffer(std::vector<T>& values)
{
	Buffer stagingBuffer;
	ShaderStorageBufferObject ssbo = createShaderStorageBuffer(sizeof(T), values.size());

	VkDeviceSize bufferSize = sizeof(T) * values.size();

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer._buffer, stagingBuffer._bufferMemory);

	void* data;
	vkMapMemory(_device.getDevice(), stagingBuffer._bufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, values.data(), (size_t)bufferSize);
	vkUnmapMemory(_device.getDevice(), stagingBuffer._bufferMemory);

	copyBuffer(stagingBuffer._buffer, ssbo._buffer, bufferSize);
	destroyBuffer(stagingBuffer);
	return ssbo;
}