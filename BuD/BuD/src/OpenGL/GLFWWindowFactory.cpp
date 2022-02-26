#include "GLFWWindowFactory.h"

#include "../OpenGL/GLFWWindow.h"

std::shared_ptr<BuD::Window> BuD::GLFWWindowFactory::CreateWindow()
{
	return std::make_shared<GLFWWindow>(BuD::ApplicationInfo());
}
