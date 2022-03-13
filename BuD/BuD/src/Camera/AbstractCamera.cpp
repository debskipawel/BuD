#include "AbstractCamera.h"

BuD::AbstractCamera::AbstractCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
	: m_position(position), m_front(), m_worldUp(),
	m_viewMatrix(),
	m_projectionMatrix(),
	m_projLeft(0.0f),
	m_projRight(5.0f)
{
	front.Normalize(m_front);
	worldUp.Normalize(m_worldUp);

	LookAt(position + front);
}

void BuD::AbstractCamera::LookAt(const dxm::Vector3& target)
{
	m_front = target - m_position;
	m_front.Normalize();

	m_front.Cross(m_worldUp, m_right);

	m_right.Cross(m_front, m_up);

	UpdateViewMatrix();

	m_pitch = 90.0f - dx::XMConvertToDegrees(acosf(m_worldUp.Dot(m_front)));
	m_yaw = dx::XMConvertToDegrees(atan2f(m_front.z, m_front.x));
}

const dxm::Matrix& BuD::AbstractCamera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const dxm::Matrix& BuD::AbstractCamera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

void BuD::AbstractCamera::Move(const dxm::Vector3& difference)
{
	m_position += (difference.x * m_right + difference.y * m_up + difference.z * m_front);

	UpdateViewMatrix();
}

void BuD::AbstractCamera::MoveTo(const dxm::Vector3& position)
{
	m_position = position;

	UpdateViewMatrix();
}

void BuD::AbstractCamera::UpdateViewMatrix()
{
	m_viewMatrix = dxm::Matrix::CreateLookAt(m_position, m_position + m_front, m_up);
}

void BuD::AbstractCamera::ProcessMouseMovement(int xOffset, int yOffset)
{
	m_yaw += xOffset * 0.1f;
	m_pitch -= yOffset * 0.1f;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	dxm::Vector3 front;

	front.x = cos(dx::XMConvertToRadians(m_yaw)) * cos(dx::XMConvertToRadians(m_pitch));
	front.y = sin(dx::XMConvertToRadians(m_pitch));
	front.z = sin(dx::XMConvertToRadians(m_yaw)) * cos(dx::XMConvertToRadians(m_pitch));
	front.Normalize(m_front);

	m_right = m_front.Cross(m_worldUp);
	m_up = m_right.Cross(m_front);

	UpdateViewMatrix();
}
