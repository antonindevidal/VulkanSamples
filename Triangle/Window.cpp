#include "Window.hpp"

#include <stdexcept>

Window::Window() :
	_size(800, 600),
	_window(nullptr)
{
}

void Window::Create(std::string& name)
{
	if (!glfwInit())
		throw std::runtime_error("Error initializing GLFW");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_window = glfwCreateWindow(_size.x, _size.y, name.c_str(), nullptr, nullptr);

	if (!_window)
		throw std::runtime_error("Error Creating Window");

	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height){ 
		// Call event manager
	});
	glfwSwapInterval(1);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::Destroy()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

glm::uvec2 Window::GetSize()
{
	int x, y;
	glfwGetWindowSize(_window, &x, &y);
	return { x,y };
}

const char** Window::getRequiredExtensions(uint32_t& extensionCount)
{
	return glfwGetRequiredInstanceExtensions(&extensionCount);
}

VkResult Window::createSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	return glfwCreateWindowSurface(instance, _window, nullptr, surface);
}
