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

struct Material
{
	GraphicsPipeline _graphicsPipeline;
	Texture _texture;
	DescriptorSet _textureDescriptor;
	DescriptorSetLayout _textureDescriptorLayout;
};