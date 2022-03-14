#include "SceneEditor.h"

#include <algorithm>
#include <limits>

#include "imgui.h"

SceneEditor::SceneEditor(std::vector<std::shared_ptr<BuD::Parameterized2DEntity>> entities, std::shared_ptr<BuD::Parameterized2DEntity> selected)
	: m_selectedEntity(selected.get()), m_entities(entities.size())
{
	std::transform(entities.begin(), entities.end(), m_entities.begin(), [](std::shared_ptr<BuD::Parameterized2DEntity> ent) { return ent.get(); });
}

void SceneEditor::RenderGui()
{
	ImGui::Begin("Main menu", &m_show);

	static int selected = -1;

	ImGui::ListBox("List of entities", &selected, [](void* ent, int idx, const char** out_text)
		{
			*out_text = "0";
			return true;
		}, reinterpret_cast<void*>(m_entities.data()), m_entities.size());

	if (selected != -1)
	{
		m_selectedEntity = m_entities[selected];

		ImGui::Text("Position:");
		ImGui::DragFloat("x: ", &m_selectedEntity->GetModel()->m_position.x, 0.01f);
		ImGui::DragFloat("y: ", &m_selectedEntity->GetModel()->m_position.y, 0.01f);
		ImGui::DragFloat("z: ", &m_selectedEntity->GetModel()->m_position.z, 0.01f);

		ImGui::Text("Rotation:");
		ImGui::DragFloat("rot x: ", &m_selectedEntity->GetModel()->m_rotation.x, 0.1f);
		ImGui::DragFloat("rot y: ", &m_selectedEntity->GetModel()->m_rotation.y, 0.1f);
		ImGui::DragFloat("rot z: ", &m_selectedEntity->GetModel()->m_rotation.z, 0.1f);

		ImGui::Text("Scale:");
		ImGui::DragFloat("scale x: ", &m_selectedEntity->GetModel()->m_scale.x, 0.01f);
		ImGui::DragFloat("scale y: ", &m_selectedEntity->GetModel()->m_scale.y, 0.01f);
		ImGui::DragFloat("scale z: ", &m_selectedEntity->GetModel()->m_scale.z, 0.01f);
	}

	ImGui::End();
}
