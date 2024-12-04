#include "VulkanBuffer.hpp"


void Buffer::create(std::shared_ptr<Context> context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(context->getDevice().getDevice(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(context->getDevice().getDevice(), _buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = context->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(context->getDevice().getDevice(), &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("Error : failed to allocate buffer memory!");
	}

	vkBindBufferMemory(context->getDevice().getDevice(), _buffer, _bufferMemory, 0);
}

void Buffer::destroy(std::shared_ptr<Context> context)
{
	vkDestroyBuffer(context->getDevice().getDevice(), _buffer, nullptr);
	vkFreeMemory(context->getDevice().getDevice(), _bufferMemory, nullptr);
}

void Buffer::copyTo(std::shared_ptr<Context> context, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = context->beginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, _buffer, dstBuffer, 1, &copyRegion);

	context->endSingleTimeCommands(commandBuffer);
}

void Buffer::copyToImage(std::shared_ptr<Context> context, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = context->beginSingleTimeCommands();
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		_buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	context->endSingleTimeCommands(commandBuffer);
}
