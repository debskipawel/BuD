#include "AbstractCamera.h"

BuD::AbstractCamera::AbstractCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
	: m_position(position), m_front(), m_worldUp(),
	m_viewMatrix(),
	m_projectionMatrix(),
	m_projLeft(-ratio),
	m_projRight(ratio)
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
	m_position += difference;
}

void BuD::AbstractCamera::MoveTo(const dxm::Vector3& position)
{
	m_position = position;
}

void BuD::AbstractCamera::UpdateViewMatrix()
{
	dx::XMFLOAT4X4 viewMatrix;

	dx::XMStoreFloat4x4(
		&viewMatrix,
		dx::XMMatrixLookAtRH(
			dx::XMLoadFloat3(&m_position),
			dx::XMVectorAdd(dx::XMLoadFloat3(&m_position), dx::XMLoadFloat3(&m_front)),
			dx::XMLoadFloat3(&m_up)
		)
	);

	m_viewMatrix = dxm::Matrix(viewMatrix);
}
