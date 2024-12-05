#include "Renderer.hpp"

Renderer::Renderer()
{
}

void Renderer::create(std::shared_ptr<Window> window, std::shared_ptr<Context> context)
{
	_window = window;
	_context = context;

	_window->addFrameBufferResizeCallback([this](int, int) {
		_framebufferResized = true;
		});

	
	auto size = window->getFrameBufferSize();
	_swapchain.create(_context, _renderPass, size.x, size.y);
	createRenderPass();
	_framebuffer.create(context, _swapchain, _renderPass);
	createCommandBuffers();
	createSyncObjects();
	_pool.create(_context, { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,50},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,50} }, 100);
}

void Renderer::destroy()
{
	_pool.destroy(_context);
	_swapchain.cleanup(_context->getDevice());

	_framebuffer.destroy(_context);
	vkDestroyRenderPass(_context->getDevice().getDevice(), _renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_context->getDevice().getDevice(), _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_context->getDevice().getDevice(), _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_context->getDevice().getDevice(), _inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(_context->getDevice().getDevice(), _context->getCommandPool(), nullptr);
}

VkCommandBuffer Renderer::getCommandBuffer()
{
	return _commandBuffers[_currentFrame];
}

uint32_t Renderer::getCurrentFrame()
{
	return _currentFrame;
}

void Renderer::startFrame()
{
	vkWaitForFences(_context->getDevice().getDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(_context->getDevice().getDevice(), _swapchain.get(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &_currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		auto size = _window->getFrameBufferSize();

		while (size.x == 0 || size.y == 0) {
			size = _window->getFrameBufferSize();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(_context->getDevice().getDevice());
		auto sCsize = _window->getFrameBufferSize();
		_swapchain.recreate(_context, _renderPass, sCsize.x, sCsize.y);
		_framebuffer.recreate(_context, _swapchain, _renderPass);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(_context->getDevice().getDevice(), 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	startRecording(_commandBuffers[_currentFrame], _currentImageIndex);
}

void Renderer::endFrame()
{
	endRecording(_commandBuffers[_currentFrame]);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_context->getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _swapchain.get() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &_currentImageIndex;
	presentInfo.pResults = nullptr; // Optional

	auto result = vkQueuePresentKHR(_context->getPresentQueue(), &presentInfo);


	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
		_framebufferResized = false;
		vkDeviceWaitIdle(_context->getDevice().getDevice());
		auto size = _window->getFrameBufferSize();
		_swapchain.recreate(_context, _renderPass, size.x, size.y);
		_framebuffer.recreate(_context, _swapchain, _renderPass);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = _swapchain.getFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_context->getDevice().getDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create render pass!");
	}
}

void Renderer::createCommandBuffers()
{
	_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _context->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

	if (vkAllocateCommandBuffers(_context->getDevice().getDevice(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to allocate command buffers!");
	}
}

void Renderer::createSyncObjects()
{
	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);


	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(_context->getDevice().getDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(_context->getDevice().getDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(_context->getDevice().getDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("Error : failed to create synchronization objects for a frame!");
		}
	}
}

void Renderer::startRecording(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to begin recording command buffer!");
	}
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _renderPass;
	renderPassInfo.framebuffer = _framebuffer.getFramebuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapchain.getExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_swapchain.getWidth());
	viewport.height = static_cast<float>(_swapchain.getHeight());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = _swapchain.getExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::endRecording(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to record command buffer!");
	}
}

// TODO : move to context
VkFormat Renderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_context->getDevice().getPhysicalDevice(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	throw std::runtime_error("Error : failed to find supported format!");
}

VkFormat Renderer::findDepthFormat()
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}
//  ----------------------

bool Renderer::hasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_context->getDevice().getPhysicalDevice(), &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Error : failed to find suitable memory type!");
}

Mesh Renderer::createMesh(const std::vector<Vertex>& vertices, const std::vector<index_t>& indices)
{
	Mesh mesh;
	mesh._vertexBuffer.create(_context, vertices);
	mesh._indexBuffer.create(_context, indices);

	mesh._nbVertices = vertices.size();
	mesh._nbIndices = indices.size();

	return mesh;
}

void Renderer::destroyMesh(Mesh& mesh)
{
	mesh._vertexBuffer.destroy(_context);
	mesh._indexBuffer.destroy(_context);
}

void Renderer::drawMesh(Mesh& mesh, Material& material, const std::vector<DescriptorSet>& descriptors)
{
	material._graphicsPipeline.bind(_context, getCommandBuffer());
	if(material._hasTexture)
		material._textureDescriptor.bind(_context, material._graphicsPipeline, getCommandBuffer(), _currentFrame, 1);

	for (auto& ds : descriptors)
	{
		ds.bind(_context, material._graphicsPipeline, getCommandBuffer(), _currentFrame, 0);
	}

	VkBuffer vertexBuffers[] = { mesh._vertexBuffer._buffer._buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(getCommandBuffer(), 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(getCommandBuffer(), mesh._indexBuffer._buffer._buffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(getCommandBuffer(), mesh._nbIndices, 1, 0, 0, 0);
}

Material Renderer::createMaterial(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath)
{
	Material m;
	m._hasTexture = true;
	m._texture.create(_context, texturePath);
	m._textureDescriptorLayout.createTextureLayout(_context, 1);
	DescriptorSetLayout layoutUB;
	layoutUB.createUniformBufferLayout(_context,0);
	m._textureDescriptor.createDescriptorSetTexture(_context, m._textureDescriptorLayout, _pool, m._texture);
	m._graphicsPipeline.create(_context, _swapchain.getWidth(), _swapchain.getHeight(), _swapchain.getExtent(), _renderPass, vertexShaderPath, fragmentShaderPath, { layoutUB._layout, m._textureDescriptorLayout._layout });
	layoutUB.destroy(_context);
	return m;
}

Material Renderer::createMaterial(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	Material m;
	m._hasTexture = false;
	DescriptorSetLayout layoutUB;
	layoutUB.createUniformBufferLayout(_context, 0);
	m._graphicsPipeline.create(_context, _swapchain.getWidth(), _swapchain.getHeight(), _swapchain.getExtent(), _renderPass, vertexShaderPath, fragmentShaderPath, { layoutUB._layout });
	layoutUB.destroy(_context);
	return m;
}

void Renderer::destroyMaterial(Material& material)
{
	material._graphicsPipeline.destroy(_context);
	if (material._hasTexture)
	{
		material._texture.destroy(_context);
		material._textureDescriptorLayout.destroy(_context);
	}
}
