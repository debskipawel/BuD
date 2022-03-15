#include "SceneEntity.h"

#include <imgui.h>

namespace BuD
{
	void SceneEntity::DrawGui()
	{
		ImGui::Text("Rotation");
		ImGui::DragFloat("r(x)", &m_rotation.x);
		ImGui::DragFloat("r(y)", &m_rotation.y);
		ImGui::DragFloat("r(z)", &m_rotation.z);

		ImGui::Text("Position");
		ImGui::DragFloat("p(x)", &m_position.x);
		ImGui::DragFloat("p(y)", &m_position.y);
		ImGui::DragFloat("p(z)", &m_position.z);

		ImGui::Text("Scale");
		ImGui::DragFloat("s(x)", &m_scale.x, 0.001f);
		ImGui::DragFloat("s(y)", &m_scale.y, 0.001f);
		ImGui::DragFloat("s(z)", &m_scale.z, 0.001f);
	}

	dxm::Matrix SceneEntity::GetModelMatrix()
	{
		auto rot = dxm::Matrix::CreateRotationX(dx::XMConvertToRadians(m_rotation.x))
			* dxm::Matrix::CreateRotationY(dx::XMConvertToRadians(m_rotation.y))
			* dxm::Matrix::CreateRotationZ(dx::XMConvertToRadians(m_rotation.z));

		auto trans = dxm::Matrix::CreateTranslation(m_position);

		auto scale = dxm::Matrix::CreateScale(m_scale);

		return scale * rot * trans;
	}
}
