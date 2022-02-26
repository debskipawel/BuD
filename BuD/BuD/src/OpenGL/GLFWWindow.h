#pragma once

#include "../Core/Window.h"
#include "../Core/ApplicationInfo.h"

#include <GLFW/glfw3.h>

namespace BuD
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const ApplicationInfo& info);
		~GLFWWindow();

		virtual void Show() override;
		virtual void ProcessEvents() override;

	private:
		GLFWwindow* m_window;
	};
}
