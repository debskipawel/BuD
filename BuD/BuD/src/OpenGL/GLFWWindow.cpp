#include "GLFWWindow.h"

#include <memory>

#include "../Event/EventEmitter.h"
#include "../Event/WindowEvents.h"

namespace BuD
{
	void window_close_callback(GLFWwindow* window)
	{
		auto e = std::make_unique<WindowClosedEvent>();
		EventEmitter::Emit(*e);
	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		auto e = std::make_unique<WindowResizedEvent>(width, height);
		EventEmitter::Emit(*e);
	}

	void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
	{
		auto e = std::make_unique<WindowMovedEvent>(xpos, ypos);
		EventEmitter::Emit(*e);
	}

	GLFWWindow::GLFWWindow(const ApplicationInfo& info)
	{
		if (!glfwInit())
		{
			printf("Sie zjebalo sie\n");
			return;
		}

		m_window = glfwCreateWindow(m_width, m_height, "Dupa", NULL, NULL);

		glfwSetWindowCloseCallback(m_window, window_close_callback);
		glfwSetWindowSizeCallback(m_window, window_size_callback);
		glfwSetWindowPosCallback(m_window, window_pos_callback);
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	
	void GLFWWindow::Show()
	{
		glfwShowWindow(m_window);
	}

	void GLFWWindow::ProcessEvents()
	{
		glfwPollEvents();
	}
}