#pragma once
#include "pch.h"

#include "backend/VulkanBackend.hpp"
#include "Components.hpp"
class Renderer
{

public:
	Renderer();

	void create(std::shared_ptr<Window> window, std::shared_ptr<Context> context);
	void destroy();

	void startFrame();
	void endFrame();

	VkCommandBuffer getCommandBuffer();
	uint32_t getCurrentFrame();

	Device& getDevice();
	void waitDeviceIdle();

public:

	// Mesh
	Mesh createMesh(const std::vector<Vertex>& vertices, const std::vector<index_t>& indices);
	void destroyMesh(Mesh& mesh);
	void drawMesh(Mesh& mesh, Material& material, const std::vector<DescriptorSet>& descriptors);

	// Material
	Material createMaterial(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath);
	Material createMaterial(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void destroyMaterial(Material& material);

private:
	std::shared_ptr<Window> _window;
	std::shared_ptr<Context> _context;

	Swapchain _swapchain;
	VkRenderPass _renderPass;
	Framebuffer _framebuffer;
	DescriptorPool _pool;


	std::vector<VkCommandBuffer> _commandBuffers;
	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;

	bool _framebufferResized = false;

	uint32_t _currentFrame = 0;
	uint32_t _currentImageIndex = 0;

	void createRenderPass();
	void createCommandBuffers();
	void createSyncObjects();

	void startRecording(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void endRecording(VkCommandBuffer commandBuffer);
};