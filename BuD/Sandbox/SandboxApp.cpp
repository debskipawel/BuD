#include "SandboxApp.h"

#include <EntryPoint.h>

using namespace DirectX::SimpleMath;

SandboxApp::SandboxApp(const BuD::DX11Device& device)
	: m_models(), m_points(), m_torus(std::make_shared<BuD::Torus>(device, 3.0f, 1.0f))
{
	m_camera = BuD::CameraFactory::MakePerspective(Vector3(0.0f, 0.0f, 3.0f), Vector3(0.0f, 0.0f, -1.0f));

	m_gui = std::make_unique<SceneEditor>(m_torus);

	m_points.emplace_back(Vector3{ 2.0f, 3.0f, 1.0f }, device);
	m_points.emplace_back(Vector3{ 5.0f, -1.0f, 1.0f }, device);
	m_points.emplace_back(Vector3{ 7.0f, 4.0f, 1.0f }, device);

	m_models.reserve(4);
	m_models.push_back(m_torus->GetModel());
	m_models.push_back(m_points[0].GetModel());
	m_models.push_back(m_points[1].GetModel());
	m_models.push_back(m_points[2].GetModel());
}

const std::vector<std::shared_ptr<BuD::Mesh>>& SandboxApp::GetModels()
{
	return m_models;
}

void SandboxApp::OnUpdate()
{
	ProcessMovement();
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

void SandboxApp::ProcessMovement()
{
	if (!m_isMoving)
	{
		return;
	}

	float dx = 0.0f, dy = 0.0f, dz = 0.0f;

	if (m_keyMap[BuD::KeyboardKeys::W])
	{
		dz += 0.01f;
	}

	if (m_keyMap[BuD::KeyboardKeys::S])
	{
		dz -= 0.01f;
	}

	if (m_keyMap[BuD::KeyboardKeys::A])
	{
		dx -= 0.01f;
	}

	if (m_keyMap[BuD::KeyboardKeys::D])
	{
		dx += 0.01f;
	}

	if (m_keyMap[BuD::KeyboardKeys::Q])
	{
		// up
		dy += 0.01f;
	}

	if (m_keyMap[BuD::KeyboardKeys::E])
	{
		// down
		dy -= 0.01f;
	}

	m_camera->Move({ dx, dy, dz });
}
