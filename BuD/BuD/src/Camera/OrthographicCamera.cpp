#include "OrthographicCamera.h"

BuD::OrthographicCamera::OrthographicCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
	: AbstractCamera(position, front, worldUp, ratio)
{
}

void BuD::OrthographicCamera::UpdateProjectionMatrix()
{
	dx::XMFLOAT4X4 projectionMatrix;

	dx::XMStoreFloat4x4(
		&projectionMatrix,
		dx::XMMatrixOrthographicRH(m_projRight - m_projLeft, m_projTop - m_projBottom, m_projNear, m_projFar)
	);

	m_projectionMatrix = dxm::Matrix(projectionMatrix);
}
