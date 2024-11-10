#pragma once

#include "pch.h"
#include "Vertex.hpp"

std::string windowName = "Vulkan triangle";


const std::vector<Vertex> vertices = {
	{{ -0.5f, -0.5f, 0.0f},	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f},	{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f},	{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f},	{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};
const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};