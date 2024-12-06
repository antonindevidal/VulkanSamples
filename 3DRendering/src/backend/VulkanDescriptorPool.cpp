#include "VulkanDescriptorPool.hpp"

void DescriptorPool::create(std::shared_ptr<Context> context, std::vector<std::pair<VkDescriptorType, uint32_t>> infos, uint32_t maxSets)
{
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

	if (vkCreateDescriptorPool(context->getDevice().getDevice(), &poolInfo, nullptr, &_pool) != VK_SUCCESS) {
		LOG_ERROR("VulkanDescriptorPool, failed to create descriptor pool !");
		throw std::runtime_error("Error : failed to create descriptor pool!");
	}
}

void DescriptorPool::destroy(std::shared_ptr<Context> context)
{
	vkDestroyDescriptorPool(context->getDevice().getDevice(), _pool, nullptr);
}
