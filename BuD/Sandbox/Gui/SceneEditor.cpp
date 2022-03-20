#include "SceneEditor.h"

#include <algorithm>
#include <limits>

#include "imgui.h"
#include "imgui_stdlib.h"

SceneEditor::SceneEditor()
	: m_selectedEntity()
{
}

void SceneEditor::RenderGui()
{
	ImGui::Begin("Main menu", &m_show);
	
	static int picked = -1;
	auto& objects = BuD::SceneObject::GetAll();
	
	std::vector<uint32_t> sceneObjects;
	std::vector<std::string*> names;
	sceneObjects.reserve(objects.size());
	names.reserve(objects.size());

	std::for_each(objects.begin(), objects.end(), [&sceneObjects, &names](std::pair<uint32_t, BuD::SceneObject*> pair) 
		{ 
			names.push_back(pair.second->Name());
			sceneObjects.push_back(pair.first); 
		}
	);

	ImGui::ListBox("Items", &picked,
		[](void* vec, int idx, const char** out_text) {
			std::vector<std::string*>* vector = reinterpret_cast<std::vector<std::string*>*>(vec);

			if (idx < 0 || idx >= vector->size())
				return false;

			*out_text = vector->at(idx)->c_str();
			return true;
		}, reinterpret_cast<void*>(&names), names.size());

	ImGui::End();

	if (picked != -1)
	{
		if (m_selectedEntity)
		{
			m_selectedEntity->Unselect();
		}

		m_selectedEntity = BuD::SceneObject::Get(sceneObjects[picked]);
		m_selectedEntity->Select();
	}

	if (!m_selectedEntity)
		return;
	
	ImGui::Begin("Secondary window");

	m_selectedEntity->DrawGui();
	ImGui::NewLine();
	ImGui::InputText("Name", names[picked]);

	ImGui::End();
}
