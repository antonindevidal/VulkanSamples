#pragma once
#include "pch.h"
#include "vulkan/vulkan.h"
#include "Buffer.hpp"

class Mesh
{
public:
	void createMesh(Device& device, const std::vector<Vertex>& vertices, const std::vector<index_t>& indices);
	void destroyMesh(Device& device);

	void drawMesh(Device& device);

private:
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;

	uint32_t _nbVertices;
	uint32_t _nbIndices;
};