#include "SandboxApp.h"
#include "Win32/Win32Window.h"

#include "EntryPoint.h"

SandboxApp::SandboxApp()
{
	m_window = std::make_shared<BuD::Win32Window>(BuD::ApplicationInfo());
}

void SandboxApp::OnUpdate()
{
	ProcessMovement();
	GetWindow()->Draw();
}

std::shared_ptr<BuD::Window> SandboxApp::GetWindow()
{
	return m_window;
}

void SandboxApp::OnConcreteEvent(BuD::WindowResizedEvent& e)
{
}

void SandboxApp::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
{
	if (e.m_button == BuD::MouseCode::RIGHT)
	{
		m_isMoving = true;
		printf("Start moving\n");
	}
}

void SandboxApp::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
{
	if (e.m_button == BuD::MouseCode::RIGHT)
	{
		m_isMoving = false;
		printf("Stop moving\n");
	}
}

void SandboxApp::OnConcreteEvent(BuD::KeyDownEvent& e)
{
	m_keyMap[e.m_key] = true;
}

void SandboxApp::OnConcreteEvent(BuD::KeyReleaseEvent& e)
{
	m_keyMap[e.m_key] = false;
}

void SandboxApp::ProcessMovement()
{
	if (!m_isMoving)
	{
		return;
	}

	if (m_keyMap[BuD::KeyboardKeys::W])
	{
		// forward
	}

	if (m_keyMap[BuD::KeyboardKeys::S])
	{
		// backwards
	}

	if (m_keyMap[BuD::KeyboardKeys::A])
	{
		// left
	}

	if (m_keyMap[BuD::KeyboardKeys::D])
	{
		// right
	}

	if (m_keyMap[BuD::KeyboardKeys::Q])
	{
		// up
	}

	if (m_keyMap[BuD::KeyboardKeys::E])
	{
		// down
	}
}
