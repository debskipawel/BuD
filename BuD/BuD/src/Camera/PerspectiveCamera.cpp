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
		m_projectionMatrix = dxm::Matrix::CreatePerspectiveFieldOfView(dx::XMConvertToRadians(m_fov), (m_projRight - m_projLeft) / (m_projTop - m_projBottom), m_projNear, m_projFar);
	}
}
