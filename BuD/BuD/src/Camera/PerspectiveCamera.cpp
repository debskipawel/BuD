#include "PerspectiveCamera.h"

namespace BuD
{
	PerspectiveCamera::PerspectiveCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
		: AbstractCamera(position, front, worldUp, ratio)
	{
		UpdateProjectionMatrix();
	}

	void BuD::PerspectiveCamera::UpdateProjectionMatrix()
	{
		dx::XMFLOAT4X4 projectionMatrix;

		dx::XMStoreFloat4x4(
			&projectionMatrix,
			dx::XMMatrixPerspectiveFovRH(dx::XMConvertToRadians(90.0f), (m_projRight - m_projLeft) / (m_projTop - m_projBottom), m_projNear, m_projFar)
		);

		m_projectionMatrix = dxm::Matrix(projectionMatrix);
	}
}
