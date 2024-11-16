#pragma once
#include "pch.h"
#include "vulkan/vulkan.h"
#include "Buffer.hpp"

struct Mesh
{
	Buffer _vertexBuffer;
	Buffer _indexBuffer;

	uint32_t _nbVertices;
	uint32_t _nbIndices;
};