#include "Window.hpp"

#include <stdexcept>

std::vector<Window::FrameBufferResizeCallback> Window::_frameBufferResizeCallbacks = {};

Window::Window() :
	_size(800, 600),
	_window(nullptr),
	_mousePosition()
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
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		for (auto& callback : _frameBufferResizeCallbacks)
		{
			callback(width, height);
		}
		});
	glfwSwapInterval(1);

	glfwSetCursorPos(_window, _size.x / 2, _size.y / 2);
	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::waitEvents()
{
	glfwWaitEvents();
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

bool Window::isKeyPressed(const unsigned int& keycode)
{
	auto state = glfwGetKey(_window, keycode);
	return state == GLFW_REPEAT || state == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(const unsigned int& keycode)
{
	auto state = glfwGetMouseButton(_window, keycode);
	return state == GLFW_REPEAT || state == GLFW_PRESS;
}

void Window::setCursorPosition(uint32_t x, uint32_t y)
{
	glfwSetCursorPos(_window, x, y);
}

glm::vec2 Window::getMousePosition()
{
	double x, y;
	glfwGetCursorPos(_window, &x, &y);
	return { x,y };
}

void Window::setCursorVisible(bool visibility)
{
	glfwSetInputMode(_window, GLFW_CURSOR, visibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

const char** Window::getRequiredExtensions(uint32_t& extensionCount)
{
	return glfwGetRequiredInstanceExtensions(&extensionCount);
}

VkResult Window::createSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	return glfwCreateWindowSurface(instance, _window, nullptr, surface);
}

glm::uvec2 Window::getFrameBufferSize()
{
	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	return { width, height };
}

void Window::addFrameBufferResizeCallback(FrameBufferResizeCallback callback)
{
	_frameBufferResizeCallbacks.push_back(callback);
}

