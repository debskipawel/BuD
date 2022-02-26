#include "SandboxApp.h"
#include "Win32/Win32Window.h"

#include "EntryPoint.h"

SandboxApp::SandboxApp()
{
	m_window = std::make_shared<BuD::Win32Window>(BuD::ApplicationInfo());
}

void SandboxApp::OnUpdate()
{
}

std::shared_ptr<BuD::Window> SandboxApp::GetWindow()
{
	return m_window;
}

void SandboxApp::OnConcreteEvent(BuD::WindowResizedEvent& e)
{
	printf("Size: %d x %d\n", e.m_width, e.m_height);
}
