#include "VulkanDescriptorSet.hpp"

void DescriptorSet::createDescriptorSetTexture(std::shared_ptr<Context> context, DescriptorSetLayout layout, DescriptorPool pool, Texture t)
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout._layout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool._pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(context->getDevice().getDevice(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
		LOG_ERROR("VulkanDescriptorSet, failed to allocate descriptor set !");
		throw std::runtime_error("Error : failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		VkDescriptorImageInfo textureInfo = t.descriptorInfo();

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = _descriptorSets[i];
		descriptorWrites[0].dstBinding = layout._binding;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pImageInfo = &textureInfo;

		vkUpdateDescriptorSets(context->getDevice().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void DescriptorSet::createDescriptorSetUniformBuffer(std::shared_ptr<Context> context, DescriptorSetLayout layout, DescriptorPool pool, UniformBuffer ub)
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout._layout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool._pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(context->getDevice().getDevice(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
		LOG_ERROR("VulkanDescriptorSet, failed to allocate descriptor set !");
		throw std::runtime_error("Error : failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		VkDescriptorBufferInfo bufferInfo = ub.descriptorInfo(i);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = _descriptorSets[i];
		descriptorWrites[0].dstBinding = layout._binding;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(context->getDevice().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void DescriptorSet::destroy(std::shared_ptr<Context> context)
{
	vkDestroyDescriptorSetLayout(context->getDevice().getDevice(), _descriptorSetLayout, nullptr);
}

void DescriptorSet::bind(std::shared_ptr<Context> context, GraphicsPipeline gp, VkCommandBuffer commandBuffer, uint32_t currentFrame, uint32_t layoutSetIndex) const
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp._pipelineLayout, layoutSetIndex, 1, &(_descriptorSets[currentFrame]), 0, nullptr);
}
