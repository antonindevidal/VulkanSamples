#pragma once
#include "../pch.h"

struct Buffer
{
	VkBuffer _buffer;
	VkDeviceMemory _bufferMemory;
};

struct UniformBuffer
{
	std::vector<VkBuffer> _buffers;
	std::vector<VkDeviceMemory> _buffersMemory;
	std::vector<void*> _buffersMapped;
	uint32_t _size;

	VkDescriptorBufferInfo descriptorInfo(int index)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _buffers[index];
		bufferInfo.offset = 0;
		bufferInfo.range = _size;
		return bufferInfo;
	}
};

struct Mesh
{
	Buffer _vertexBuffer;
	Buffer _indexBuffer;

	uint32_t _nbVertices;
	uint32_t _nbIndices;
};

struct Texture
{
	VkImage _textureImage;
	VkDeviceMemory _textureImageMemory;
	VkImageView _textureImageView;
	VkSampler _textureSampler;

	VkDescriptorImageInfo descriptorInfo()
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = _textureImageView;
		imageInfo.sampler = _textureSampler;
		return imageInfo;
	}
};

using DescriptorPool = VkDescriptorPool;
using DescriptorSetLayout = VkDescriptorSetLayout;

struct DescriptorSet
{
	VkDescriptorSetLayout _descriptorSetLayout;
	std::vector<VkDescriptorSet> _descriptorSets;
};

struct GraphicsPipeline
{
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
};

using index_t = uint16_t;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};


namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}