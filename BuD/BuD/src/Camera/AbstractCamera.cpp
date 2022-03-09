#include "AbstractCamera.h"

BuD::AbstractCamera::AbstractCamera(const dx::XMFLOAT3& position, const dx::XMFLOAT3& front, const dx::XMFLOAT3& worldUp, float ratio)
	: m_position(position), m_front(), m_worldUp(),
	m_viewMatrix(),
	m_projectionMatrix(),
	m_projLeft(-ratio),
	m_projRight(ratio)
{
	dx::XMStoreFloat3(
		&m_front,
		dx::XMVector3Normalize(dx::XMLoadFloat3(&front))
	);

	dx::XMStoreFloat3(
		&m_worldUp,
		dx::XMVector3Normalize(dx::XMLoadFloat3(&worldUp))
	);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void BuD::AbstractCamera::LookAt(const dx::XMFLOAT3& target)
{
	dx::XMStoreFloat3(
		&m_front,
		dx::XMVector3Normalize(
			dx::XMVectorSubtract(dx::XMLoadFloat3(&target), dx::XMLoadFloat3(&m_position))
		)
	);

	dx::XMStoreFloat3(
		&m_right,
		dx::XMVector3Cross(dx::XMLoadFloat3(&m_front), dx::XMLoadFloat3(&m_worldUp))
	);

	dx::XMStoreFloat3(
		&m_up,
		dx::XMVector3Cross(dx::XMLoadFloat3(&m_right), dx::XMLoadFloat3(&m_front))
	);

	UpdateViewMatrix();

	m_pitch = 90.0f - dx::XMConvertToDegrees(acosf(dx::XMVector3Dot(dx::XMLoadFloat3(&m_worldUp), dx::XMLoadFloat3(&m_front)).m128_f32[0]));
	m_yaw = dx::XMConvertToDegrees(atan2f(m_front.z, m_front.x));
}

dx::XMMATRIX BuD::AbstractCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

dx::XMMATRIX BuD::AbstractCamera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void BuD::AbstractCamera::Move(const dx::XMFLOAT3& difference)
{
	dx::XMStoreFloat3(
		&m_position,
		dx::XMVectorAdd(dx::XMLoadFloat3(&m_position), dx::XMLoadFloat3(&difference))
	);
}

void BuD::AbstractCamera::MoveTo(const dx::XMFLOAT3& position)
{
	m_position = position;
}

void BuD::AbstractCamera::UpdateViewMatrix()
{
	m_viewMatrix = dx::XMMatrixLookAtRH(
		dx::XMLoadFloat3(&m_position),
		dx::XMVectorAdd(dx::XMLoadFloat3(&m_position), dx::XMLoadFloat3(&m_front)),
		dx::XMLoadFloat3(&m_up)
	);
}
