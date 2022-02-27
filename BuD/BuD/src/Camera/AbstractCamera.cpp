#include "AbstractCamera.h"

#include "../gmtl/gmtl.h"

BuD::AbstractCamera::AbstractCamera(const Vector3D& position, const Vector3D& front, const Vector3D& worldUp, float ratio)
	: m_position(position),
	m_front(front),
	m_worldUp(worldUp.Normalized()),
	m_viewMatrix(),
	m_projectionMatrix(),
	m_projLeft(-ratio),
	m_projRight(ratio)
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void BuD::AbstractCamera::LookAt(const Vector3D& target)
{
	m_front = (target - m_position).Normalized();
	m_right = m_front.CrossProduct(m_worldUp);
	m_up = m_right.CrossProduct(m_front);

	UpdateViewMatrix();
}

BuD::Matrix3D BuD::AbstractCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

BuD::Matrix3D BuD::AbstractCamera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void BuD::AbstractCamera::Move(const Vector3D& difference)
{
	m_position += difference;
}

void BuD::AbstractCamera::MoveTo(const Vector3D& position)
{
	m_position = position;
}

void BuD::AbstractCamera::UpdateViewMatrix()
{
	float data[] = {
		m_right[0], m_up[0], m_front[0], m_position[0],
		m_right[1], m_up[1], m_front[1], m_position[1],
		m_right[2], m_up[2], m_front[2], m_position[2],
		0.0f, 0.0f, 0.0f, 1.0f
	};

	m_viewMatrix = Matrix3D(data);
}
