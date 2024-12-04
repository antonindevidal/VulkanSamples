#pragma once
#include "../pch.h"
#include "VulkanContext.hpp"
#include "VulkanBuffer.hpp"
#include "VkStructs.hpp"

struct Texture
{
	VkImage _image;
	VkDeviceMemory _imageMemory;
	VkImageView _imageView;
	VkSampler _sampler;

	void create(std::shared_ptr<Context> context, const std::string path);
	void destroy(std::shared_ptr<Context> context);

	void createImage(std::shared_ptr<Context> context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void createImageView(std::shared_ptr<Context> context, VkFormat format, VkImageAspectFlags aspectFlags);
	void createTextureSampler(std::shared_ptr<Context> context);
	void transitionImageLayout(std::shared_ptr<Context> context, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	bool hasStencilComponent(VkFormat format);

	VkDescriptorImageInfo descriptorInfo()
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = _imageView;
		imageInfo.sampler = _sampler;
		return imageInfo;
	}
};
