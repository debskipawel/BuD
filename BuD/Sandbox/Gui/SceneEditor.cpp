#include "SceneEditor.h"

#include <algorithm>
#include <limits>

#include "imgui.h"

SceneEditor::SceneEditor(std::shared_ptr<BuD::Parameterized2DEntity> selected)
	: m_selectedEntity(selected)
{
}

void SceneEditor::RenderGui()
{
	ImGui::Begin("Main menu", &m_show);

	m_selectedEntity->DrawGui();

	ImGui::End();
}
