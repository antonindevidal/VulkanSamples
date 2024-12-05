#include "VulkanFramebuffer.hpp"

void Framebuffer::create(std::shared_ptr<Context> context, Swapchain& swapchain, VkRenderPass renderPass )
{
	_swapChainFramebuffers.resize(swapchain._swapChainImageViews.size());
	for (size_t i = 0; i < swapchain._swapChainImageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			swapchain._swapChainImageViews[i],
			swapchain._depthResource._imageView
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

		if (vkCreateFramebuffer(context->getDevice().getDevice(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Error : failed to create framebuffer!");
		}
	}
}

void Framebuffer::destroy(std::shared_ptr<Context> context)
{
	for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(context->getDevice().getDevice(), _swapChainFramebuffers[i], nullptr);
	}
}

void Framebuffer::recreate(std::shared_ptr<Context> context, Swapchain& swapchain, VkRenderPass renderPass)
{
	destroy(context);
	create(context, swapchain, renderPass);
}

VkFramebuffer Framebuffer::getFramebuffer(uint32_t index)
{
	return _swapChainFramebuffers[index];
}
