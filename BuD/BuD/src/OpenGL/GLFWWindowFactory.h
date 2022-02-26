#pragma once

#include <memory>

#include "../Core/Window.h"

namespace BuD
{
	class GLFWWindowFactory
	{
	public:
		static std::shared_ptr<Window> CreateWindow();
	protected:
		GLFWWindowFactory() = default;
	};
}