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
		ImGui::BeginChild("Modify selected");

		m_selectedEntity = m_entities[selected];

		m_selectedEntity->DrawGui();

		ImGui::EndChild();
	}

	ImGui::End();
}
