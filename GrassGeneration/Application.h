#pragma once

#include "pch.h"
#include "Vertex.hpp"

std::string windowName = "Vulkan Grass Generation";

static const uint32_t NB_BLADES = 10000;
static const float CAMERA_SPEED= 5;
static const float CAMERA_SENSITIVITY= 0.1;



struct Camera
{
	glm::mat4 view;
	glm::vec3 position = { 10.0f, 10.0f, 5.0f };
	glm::vec3 front = { -1.0, 0.0, 0.0 };
	glm::vec3 up = { 0.0, 0.0, 1.0 };
	float yaw = 45.0f;
	float pitch = 0.0f;
};

struct GrassBlade {
	glm::vec4 position; //x, y, z, Zrotation
	glm::vec4 data; // heightFactor, stiffness
};
struct GrassBladeData {
	glm::vec4 data; // x, y, width, nbBlades
	std::array<GrassBlade, NB_BLADES> positions; // x, y, z, Zrotation
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
	alignas(16) float time;
};
