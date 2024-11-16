#pragma once

#include "pch.h"
#include "Vertex.hpp"

std::string windowName = "Vulkan triangle";


struct GrassBladeData {
	glm::vec4 position;
};

const std::vector<Vertex> verticesGround = {
	{{ -10.0f, -10.0f, 0.0f},	{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{10.0f, -10.0f, 0.0f},	{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{10.0f, 10.0f, 0.0f},	{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-10.0f, 10.0f, 0.0f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indicesGround = {
	0, 1, 2, 2, 3, 0
};


struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};
