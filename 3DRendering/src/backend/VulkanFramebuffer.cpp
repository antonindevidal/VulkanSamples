#include "VulkanFramebuffer.hpp"

void Framebuffer::create(Device& device, Swapchain& swapchain, VkRenderPass renderPass )
{
	
	_swapChainFramebuffers.resize(swapchain._swapChainImageViews.size());
	for (size_t i = 0; i < swapchain._swapChainImageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			swapchain._swapChainImageViews[i],
			swapchain._depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchain.getWidth();
		framebufferInfo.height = swapchain.getHeight();
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Error : failed to create framebuffer!");
		}
	}
}

void Framebuffer::destroy(Device& device)
{
	for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(device.getDevice(), _swapChainFramebuffers[i], nullptr);
	}
}

VkFramebuffer Framebuffer::getFramebuffer(uint32_t index)
{
	return _swapChainFramebuffers[index];
}
