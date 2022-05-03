#include "StereoscopicCamera.h"

#include <imgui.h>

namespace BuD
{
	StereoscopicCamera::StereoscopicCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
		: AbstractCamera(position, front, worldUp, ratio)
	{
		UpdateProjectionMatrix();
	}

	void FixValues(float* ref, float min, float max)
	{
		*ref = min(max(*ref, min), max);
	}

	void StereoscopicCamera::DrawGui()
	{
		const float minFov = 45.0f, maxFov = 120.0f;
		const float minNear = 0.001f, maxNear = 10.0f;
		const float minEye = 0.0f;
		const float minFocus = 1.0f, maxFocus = 120.0f;

		ImGui::Text("FOV:");
		ImGui::DragFloat("##FOV", &m_fov, 1.0f, minFov, maxFov);
		
		ImGui::Text("Near:");
		ImGui::DragFloat("##Near", &m_projNear, 0.1f, minNear, maxNear);

		ImGui::Text("Eye dist:");
		ImGui::DragFloat("##d", &m_eyeDistance, 0.1f, minEye, m_focus);
		
		ImGui::Text("Focus plane:");
		ImGui::DragFloat("##focus_plane", &m_focus, 0.5f, minFocus, maxFocus);

		m_fov = min(max(m_fov, minFov), maxFov);
		m_focus = max(m_focus, minFocus);
		m_projNear = min(max(m_projNear, minNear), maxNear);
		m_eyeDistance = min(max(m_eyeDistance, minEye), m_focus);

		UpdateProjectionMatrix();
	}

	void StereoscopicCamera::UpdateProjectionMatrix()
	{
		m_projectionMatrix = dxm::Matrix::CreatePerspectiveFieldOfView(dx::XMConvertToRadians(m_fov), (m_projRight - m_projLeft) / (m_projTop - m_projBottom), m_projNear, m_projFar);

		float halfDistance = m_eyeDistance / 2;

		float top = m_focus * tanf(dx::XMConvertToRadians(m_fov / 2.0f));
		float bottom = m_focus * tanf(-dx::XMConvertToRadians(m_fov / 2.0f));
		float width = (top - bottom) * (m_projRight - m_projLeft) / (m_projTop - m_projBottom);
		float L = -width / 2;
		float R = width / 2;

		float leftL = (L + halfDistance) * m_projNear / m_focus - halfDistance;
		float leftR = (R + halfDistance) * m_projNear / m_focus - halfDistance;

		float rightL = (L - halfDistance) * m_projNear / m_focus + halfDistance;
		float rightR = (R - halfDistance) * m_projNear / m_focus + halfDistance;

		top *= m_projNear / m_focus;
		bottom *= m_projNear / m_focus;

		m_leftEyeProjection = dxm::Matrix::CreatePerspectiveOffCenter(leftL, leftR, bottom, top, m_projNear, m_projFar);
		m_rightEyeProjection = dxm::Matrix::CreatePerspectiveOffCenter(rightL, rightR, bottom, top, m_projNear, m_projFar);
	}
}
