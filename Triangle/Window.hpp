#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "pch.h"

class Window
{
public:

	Window();
	Window(Window& w) = delete;
	Window& operator=(const Window&) = delete;
	
	void Create(std::string& name);
	bool ShouldClose();
	void PollEvents();
	void Destroy();
	glm::uvec2 GetSize();

	const char** getRequiredExtensions(uint32_t& extensionCount);
	HWND getWin32Window();
	VkResult createSurface(VkInstance instance, VkSurfaceKHR* surface);
	glm::uvec2 getFrameBufferSize();

private:
	glm::uvec2 _size;
	GLFWwindow* _window;
};

