#include "SandboxApp.h"

#include <EntryPoint.h>

using namespace DirectX::SimpleMath;

SandboxApp::SandboxApp(const BuD::DX11Device& device)
	: m_points(), m_torus(std::make_shared<BuD::Torus>(device, 3.0f, 1.0f))
{
	m_camera = BuD::CameraFactory::MakePerspective(Vector3(0.0f, 0.0f, 3.0f), Vector3(0.0f, 0.0f, -1.0f));

	m_gui = std::make_unique<SceneEditor>();

	m_points.push_back(std::make_shared<BuD::Point>(Vector3{ 2.0f, 3.0f, 1.0f }, device));
	m_points.push_back(std::make_shared<BuD::Point>(Vector3{ 5.0f, -1.0f, 1.0f }, device));
	m_points.push_back(std::make_shared<BuD::Point>(Vector3{ 7.0f, 4.0f, 1.0f }, device));
}


void SandboxApp::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
}

void SandboxApp::OnGuiRender()
{
	m_gui->RenderGui();
}

void SandboxApp::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
{
	if (e.m_button == BuD::MouseCode::RIGHT)
	{
		m_isMoving = true;
	}
}

void SandboxApp::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
{
	if (e.m_button == BuD::MouseCode::RIGHT)
	{
		m_isMoving = false;
	}
}

void SandboxApp::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
	if (m_isMoving)
	{
		m_camera->ProcessMouseMovement(e.m_xOffset, e.m_yOffset);
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

void SandboxApp::OnConcreteEvent(BuD::WindowResizedEvent& e)
{
	m_camera->UpdateAspectRatio(static_cast<float>(e.m_width) / e.m_height);
}

void SandboxApp::ProcessMovement(float deltaTime)
{
	if (!m_isMoving)
	{
		return;
	}

	float dx = 0.0f, dy = 0.0f, dz = 0.0f;

	if (m_keyMap[BuD::KeyboardKeys::W])
	{
		dz += 1.0f;
	}

	if (m_keyMap[BuD::KeyboardKeys::S])
	{
		dz -= 1.0f;
	}

	if (m_keyMap[BuD::KeyboardKeys::A])
	{
		dx -= 1.0f;
	}

	if (m_keyMap[BuD::KeyboardKeys::D])
	{
		dx += 1.0f;
	}

	if (m_keyMap[BuD::KeyboardKeys::Q])
	{
		// up
		dy += 1.0f;
	}

	if (m_keyMap[BuD::KeyboardKeys::E])
	{
		// down
		dy -= 1.0f;
	}

	m_camera->Move(Vector3{ dx, dy, dz } * 3.0f * deltaTime);
}
