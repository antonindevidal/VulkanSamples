#pragma once

#include "pch.h"
#include "VkStructs.hpp"

std::string windowName = "Vulkan triangle";


const std::vector<Vertex> vertices = {
	{{ -0.5f, -0.5f, 0.0f},	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f},	{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f},	{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<Vertex> vertices2 = {
	{ {-0.5f, -0.5f, -0.5f},{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
	{ {0.5f, -0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
	{ {0.5f, 0.5f, -0.5f},	{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
	{ {-0.5f, 0.5f, -0.5f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }
};

const std::vector<Vertex> vertices3 = {
	{ {-0.5f, -0.5f, 0.5f},{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
	{ {0.5f, -0.5f, 0.5f},	{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
	{ {0.5f, 0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
	{ {-0.5f, 0.5f, 0.5f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};
const std::vector<uint16_t> indices2 = {
	0, 1, 2, 2, 3, 0
};
const std::vector<uint16_t> indices3 = {
	0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};
