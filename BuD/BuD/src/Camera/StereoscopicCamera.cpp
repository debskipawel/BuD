#include "StereoscopicCamera.h"

namespace BuD
{
	StereoscopicCamera::StereoscopicCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
		: AbstractCamera(position, front, worldUp, ratio)
	{
		UpdateProjectionMatrix();
	}

	void StereoscopicCamera::UpdateProjectionMatrix()
	{
		m_projectionMatrix = dxm::Matrix::CreatePerspectiveFieldOfView(dx::XMConvertToRadians(m_fov), (m_projRight - m_projLeft) / (m_projTop - m_projBottom), m_projNear, m_projFar);

		float halfDistance = m_eyeDistance / 2;

		float leftL = (m_projLeft * m_focus / m_projFar + halfDistance) * m_projNear / m_focus;
		float leftR = (m_projRight * m_focus / m_projFar + halfDistance) * m_projNear / m_focus;

		float rightL = (m_projLeft * m_focus / m_projFar - halfDistance) * m_projNear / m_focus;
		float rightR = (m_projRight * m_focus / m_projFar - halfDistance) * m_projNear / m_focus;

		float top = m_projTop * m_projNear / m_projFar;
		float bottom = m_projBottom * m_projNear / m_projFar;

		m_leftEyeProjection = dxm::Matrix::CreatePerspectiveOffCenter(leftL, leftR, bottom, top, m_projNear, m_projFar);
		m_rightEyeProjection = dxm::Matrix::CreatePerspectiveOffCenter(rightL, rightR, bottom, top, m_projNear, m_projFar);
	}
}
