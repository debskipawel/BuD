#include "SandboxApp.h"

#include <EntryPoint.h>

using namespace DirectX::SimpleMath;

SandboxApp::SandboxApp()
{
	m_camera = BuD::CameraFactory::MakeOrthographic(Vector3(0.0f, 0.0f, 5.0f), Vector3(0.0f, 0.0f, -1.0f));
}

void SandboxApp::OnUpdate()
{
	ProcessMovement();
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
