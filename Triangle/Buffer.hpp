#pragma once
#include "pch.h"
#include "Device.hpp"
#include "Vertex.hpp"

class Buffer
{
public:	
	virtual ~Buffer() = 0;

	void destroyBuffer(Device& device);

	VkBuffer getBuffer();

protected:
	VkBuffer _buffer;
	VkDeviceMemory _bufferMemory;

	void createBuffer(Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(Device& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);


	uint32_t findMemoryType(Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

class VertexBuffer : public Buffer
{
public:
	void createVertexBuffer(Device& device, std::vector<Vertex> vertices);
};

class IndexBuffer: public Buffer
{
public:
	void createIndexBuffer(Device& device, std::vector<index_t>);
};