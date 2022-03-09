#include "OrthographicCamera.h"

BuD::OrthographicCamera::OrthographicCamera(const dx::XMFLOAT3& position, const dx::XMFLOAT3& front, const dx::XMFLOAT3& worldUp, float ratio)
	: AbstractCamera(position, front, worldUp, ratio)
{
}

void BuD::OrthographicCamera::UpdateProjectionMatrix()
{
	m_projectionMatrix = dx::XMMatrixOrthographicRH(m_projRight - m_projLeft, m_projTop - m_projBottom, m_projNear, m_projFar);
}
