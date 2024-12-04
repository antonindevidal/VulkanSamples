#include "Renderer.hpp"

Renderer::Renderer()
{
}

void Renderer::createRenderer(std::shared_ptr<Window> window, std::shared_ptr<Context> context)
{
	_window = window;
	_context = context;

	_window->addFrameBufferResizeCallback([this](int, int) {
		_framebufferResized = true;
		});

	
	auto size = window->getFrameBufferSize();
	_swapchain.create(_context, _renderPass, size.x, size.y);
	createRenderPass();
	_framebuffer.create(context->getDevice(), _swapchain, _renderPass);
	createCommandBuffers();
	createSyncObjects();
	_pool = createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,50},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,50} }, 100);
}

void Renderer::destroy()
{
	destroyDescriptorPool(_pool);
	_swapchain.cleanup(_context->getDevice());

	_framebuffer.destroy(_context->getDevice());
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
		_framebuffer.destroy(_context->getDevice());
		_swapchain.recreate(_context, _renderPass, sCsize.x, sCsize.y);
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
		_framebuffer.destroy(_context->getDevice());
		auto size = _window->getFrameBufferSize();
		_swapchain.recreate(_context, _renderPass, size.x, size.y);
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
	bindDescriptorSet(material._textureDescriptor, material._graphicsPipeline, 1);

	for (auto& ds : descriptors)
	{
		bindDescriptorSet(ds, material._graphicsPipeline, 0);
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
	m._texture.create(_context, texturePath);
	m._textureDescriptorLayout = createDescriptorSetlayoutTexture(1);
	DescriptorSetLayout layoutUB = createDescriptorSetlayoutUb(0);
	m._textureDescriptor = createDescriptorSet(m._textureDescriptorLayout, _pool, m._texture);
	m._graphicsPipeline.create(_context, _swapchain.getWidth(), _swapchain.getHeight(), _swapchain.getExtent(), _renderPass, vertexShaderPath, fragmentShaderPath, { layoutUB._layout, m._textureDescriptorLayout._layout });
	destroyDescriptorSetLayout(layoutUB);
	return m;
}

void Renderer::destroyMaterial(Material& material)
{
	material._graphicsPipeline.destroy(_context);
	material._texture.destroy(_context);
	destroyDescriptorSetLayout(material._textureDescriptorLayout);
}

DescriptorPool Renderer::createDescriptorPool(std::vector<std::pair<VkDescriptorType, uint32_t>> infos, uint32_t maxSets)
{
	VkDescriptorPool pool;

	std::vector<VkDescriptorPoolSize> poolSizes{};
	poolSizes.resize(infos.size());

	for (int i = 0; i < infos.size(); i++)
	{
		poolSizes[i].type = infos[i].first;
		poolSizes[i].descriptorCount = infos[i].second * static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = maxSets * static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(_context->getDevice().getDevice(), &poolInfo, nullptr, &pool) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create descriptor pool!");
	}
	return pool;
}

void Renderer::destroyDescriptorPool(DescriptorPool pool)
{
	vkDestroyDescriptorPool(_context->getDevice().getDevice(), pool, nullptr);
}

DescriptorSetLayout Renderer::createDescriptorSetlayoutTexture(uint32_t binding)
{
	DescriptorSetLayout layout;
	layout._binding = binding;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = binding;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(_context->getDevice().getDevice(), &layoutInfo, nullptr, &layout._layout) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create descriptor set layout!");
	}

	return layout;
}

DescriptorSetLayout Renderer::createDescriptorSetlayoutUb(uint32_t binding)
{
	DescriptorSetLayout layout;
	layout._binding = binding;

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = binding;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(_context->getDevice().getDevice(), &layoutInfo, nullptr, &layout._layout) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create descriptor set layout!");
	}

	return layout;
}

void Renderer::destroyDescriptorSetLayout(DescriptorSetLayout layout)
{
	vkDestroyDescriptorSetLayout(_context->getDevice().getDevice(), layout._layout, nullptr);
}

DescriptorSet Renderer::createDescriptorSet(DescriptorSetLayout layout, DescriptorPool pool, Texture t)
{
	DescriptorSet set;

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout._layout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	set._descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(_context->getDevice().getDevice(), &allocInfo, set._descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		VkDescriptorImageInfo textureInfo = t.descriptorInfo();

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = set._descriptorSets[i];
		descriptorWrites[0].dstBinding = layout._binding;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pImageInfo = &textureInfo;

		vkUpdateDescriptorSets(_context->getDevice().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
	return set;
}

DescriptorSet Renderer::createDescriptorSet(DescriptorSetLayout layout, DescriptorPool pool, UniformBuffer ub)
{
	DescriptorSet set;


	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout._layout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	set._descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(_context->getDevice().getDevice(), &allocInfo, set._descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		VkDescriptorBufferInfo bufferInfo = ub.descriptorInfo(i);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = set._descriptorSets[i];
		descriptorWrites[0].dstBinding = layout._binding;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		try
		{
			vkUpdateDescriptorSets(_context->getDevice().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
		catch (std::exception e)
		{
			std::cout << e.what();
			throw std::runtime_error("aaaaaaaaaaa");
		}
	}
	return set;
}

void Renderer::destroyDescriptorSet(DescriptorSet descriptorSet)
{
	vkDestroyDescriptorSetLayout(_context->getDevice().getDevice(), descriptorSet._descriptorSetLayout, nullptr);
}

void Renderer::bindDescriptorSet(DescriptorSet descriptorSet, GraphicsPipeline gp, uint32_t index)
{
	if (_currentGraphicsPipeline._graphicsPipeline = VK_NULL_HANDLE)
	{
		throw std::runtime_error("Error : Can't bind Descriptor set if no graphcis pipeline bound !");
	}
	vkCmdBindDescriptorSets(getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, gp._pipelineLayout, index, 1, &(descriptorSet._descriptorSets[_currentFrame]), 0, nullptr);
}
