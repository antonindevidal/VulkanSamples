#pragma once 
#include "pch.h"
#include "backend/VulkanBackend.hpp"

struct Mesh
{
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;

	uint32_t _nbVertices;
	uint32_t _nbIndices;
};