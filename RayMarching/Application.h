#pragma once

#include "pch.h"
#include "VkStructs.hpp"

std::string windowName = "Vulkan Ray Marching";

// Camera parameters
static const float CAMERA_SPEED = 5;
static const float CAMERA_SENSITIVITY = 0.1;
static const float CAMERA_FOV = 45.0;

struct Camera
{
	glm::mat4 view;
	glm::vec3 position = { 0.0f, -3.0f, 0.0f };
	glm::vec3 front = { 0.0, 1.0, 0.0 };
	glm::vec3 up = { 0.0, 0.0, 1.0 };
	float yaw = 45.0f;
	float pitch = 0.0f;
};

const std::vector<Vertex> verticesGround = {
	{{ -10.0f, -10.0f, 0.0f},	{0.0f, 0.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 0.0f}},
	{{10.0f, -10.0f, 0.0f},		{0.0f, 0.0f, 1.0f}, {0.08f, 0.37f, 0.11f}, {0.0f, 0.0f}},
	{{10.0f, 10.0f, 0.0f},		{0.0f, 0.0f, 1.0f}, {0.08f, 0.37f, 0.11f}, {0.0f, 1.0f}},
	{{-10.0f, 10.0f, 0.0f},		{0.0f, 0.0f, 1.0f}, {0.08f, 0.37f, 0.11f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indicesGround = {
	0, 1, 2, 2, 3, 0
};

const std::vector<Vertex> verticesSkybox = {
	// Bot
	{{ -1.0f, -1.0f, -1.0f},{0.0f, 0.0f, 1.0f},	{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{1.0f, -1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},	{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{1.0f, 1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},	{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-1.0f, 1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},	{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	// Top
	{{ -1.0f, -1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{1.0f, -1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-1.0f, 1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	// Front
	{{1.0f, -1.0f, -1.0f }, {0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{1.0f, 1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{1.0f, -1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	// Back
	{{-1.0f, -1.0f, -1.0f },{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{-1.0f, 1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{-1.0f, 1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	// Left
	{{-1.0f, -1.0f, -1.0f },{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{1.0f, -1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{1.0f, -1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	// Right
	{{-1.0f, 1.0f, -1.0f }, {0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{1.0f, 1.0f, -1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-1.0f, 1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
};

const std::vector<uint16_t> indicesSkybox = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4,
	8, 9, 10, 10 ,11, 8,
	12, 13, 14, 14, 15, 12,
	16, 17, 18, 18, 19, 16,
	20, 21, 22, 22, 23, 20
};

const std::vector<Vertex> verticesSSRect = {
	{{-1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 0.0f}},
	{{ 1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, 1.0f}, {0.08f, 0.37f, 0.11f}, {1.0f, 0.0f}},
	{{ 1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, 1.0f}, {0.08f, 0.37f, 0.11f}, {1.0f, 1.0f}},
	{{-1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, 1.0f}, {0.08f, 0.37f, 0.11f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indicesSSRect = {
	0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec4 directionnalLight;
	alignas(16) glm::vec4 cameraFront;
	alignas(16) glm::vec4 cameraPos;
	alignas(16) glm::vec4 windowData; //width, height, fov
	alignas(16) float time;
};
