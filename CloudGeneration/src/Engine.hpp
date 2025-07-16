#pragma once
#include "pch.h"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Logger.hpp"
#include "backend/VulkanBackend.hpp"

// To remove
const std::vector<Vertex> vertices = {
	{{ -0.5f, -0.5f, 0.0f},	{1.0f, 0.0f, 0.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f},	{0.0f, 1.0f, 0.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f},	{0.0f, 0.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f},	{1.0f, 1.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 1.0f}}
};

const std::vector<Vertex> vertices2 = {
	{ {-0.5f, -0.5f, -0.5f},{1.0f, 0.0f, 0.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 0.0f} },
	{ {0.5f, -0.5f, -0.5f},	{0.0f, 1.0f, 0.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 0.0f} },
	{ {0.5f, 0.5f, -0.5f},	{0.0f, 0.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 1.0f} },
	{ {-0.5f, 0.5f, -0.5f},	{1.0f, 1.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 1.0f} }
};

const std::vector<Vertex> vertices3 = {
	{ {-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 0.0f} },
	{ {0.5f, -0.5f, 0.5f},	{0.0f, 1.0f, 0.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 0.0f} },
	{ {0.5f, 0.5f, 0.5f},	{0.0f, 0.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {0.0f, 1.0f} },
	{ {-0.5f, 0.5f, 0.5f},	{1.0f, 1.0f, 1.0f},	{0.08f, 0.37f, 0.11f}, {1.0f, 1.0f} }
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

class Engine
{
public:
	Engine();
	~Engine();

	void run();

	inline static Engine* get() { return s_Instance; }

	inline std::shared_ptr<Window> getWindow() { return _window; }
	inline std::shared_ptr<Renderer> getRenderer() { return _renderer; }

private:
	std::shared_ptr<Window> _window;
	std::shared_ptr<Context> _context;
	std::shared_ptr<Renderer> _renderer;


private:
	static Engine* s_Instance;
};