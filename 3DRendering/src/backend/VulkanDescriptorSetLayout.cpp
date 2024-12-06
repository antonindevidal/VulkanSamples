#include "VulkanDescriptorSetLayout.hpp"

void DescriptorSetLayout::createUniformBufferLayout(std::shared_ptr<Context> context, uint32_t binding)
{
	_binding = binding;

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

	if (vkCreateDescriptorSetLayout(context->getDevice().getDevice(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS) {
		LOG_ERROR("VulkanDescriptorSetLayout, failed to allocate descriptor set layout !");
		throw std::runtime_error("Error : failed to create descriptor set layout!");
	}
}

void DescriptorSetLayout::createTextureLayout(std::shared_ptr<Context> context, uint32_t binding)
{
	_binding = binding;

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

	if (vkCreateDescriptorSetLayout(context->getDevice().getDevice(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS) {
		LOG_ERROR("VulkanDescriptorSetLayout, failed to allocate descriptor set layout !");
		throw std::runtime_error("Error : failed to create descriptor set layout!");
	}
}

void DescriptorSetLayout::destroy(std::shared_ptr<Context> context)
{
	vkDestroyDescriptorSetLayout(context->getDevice().getDevice(), _layout, nullptr);
}

