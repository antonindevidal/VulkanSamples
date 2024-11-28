#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "pch.h"

class Window
{

	using FrameBufferResizeCallback = std::function<void(int, int)>;
public:

	Window();
	Window(Window& w) = delete;
	Window& operator=(const Window&) = delete;

	void Create(std::string& name);
	bool ShouldClose();
	void PollEvents();
	void waitEvents();
	void Destroy();
	glm::uvec2 GetSize();

	bool isKeyPressed(const unsigned int& keycode);

	void setCursorPosition(uint32_t x, uint32_t y);
	glm::vec2 getMousePosition();

	const char** getRequiredExtensions(uint32_t& extensionCount);
	HWND getWin32Window();
	VkResult createSurface(VkInstance instance, VkSurfaceKHR* surface);
	glm::uvec2 getFrameBufferSize();

	void addFrameBufferResizeCallback(FrameBufferResizeCallback callback);

private:
	glm::uvec2 _size;
	GLFWwindow* _window;

	glm::uvec2 _mousePosition;

	static std::vector<FrameBufferResizeCallback> _frameBufferResizeCallbacks;
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

