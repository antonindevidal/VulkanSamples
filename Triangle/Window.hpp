#pragma once 
#include "pch.h"
#include <GLFW/glfw3.h>

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

private:
	glm::uvec2 _size;
	GLFWwindow* _window;
};

