#include "ObjectsEditor.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "Geometry/Point.h"
#include "Geometry/Torus.h"

#include <algorithm>

namespace BuD
{
	ObjectsEditor::ObjectsEditor(std::vector<std::shared_ptr<SceneObject>>& objects)
		: m_objects(objects)
	{
	}
	
	void ObjectsEditor::DrawGui(const DX11Device& device)
	{
		ImGui::Begin("Main settings");

		ImGui::Text("Cursor position");
		ImGui::DragFloat("X", &m_cursorPosition.x, 0.1f);
		ImGui::DragFloat("Y", &m_cursorPosition.y, 0.1f);
		ImGui::DragFloat("Z", &m_cursorPosition.z, 0.1f);

		ImGui::NewLine();

		if (ImGui::Button("Add torus"))
		{
			auto torus = std::make_shared<Torus>(device, 3.0f, 1.0f);
			torus->GetModel()->m_position = m_cursorPosition;

			m_objects.push_back(torus);
		}

		if (ImGui::Button("Add point"))
		{
			m_objects.push_back(
				std::make_shared<Point>(m_cursorPosition, device)
			);
		}

		ImGui::End();

		ImGui::Begin("Scene objects");

		static int picked = -1;
		auto& objects = SceneObject::GetAll();

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
			if (m_selectedObject)
			{
				m_selectedObject->Unselect();
			}

			m_selectedObject = SceneObject::Get(sceneObjects[picked]);
			m_selectedObject->Select();
			
			ImGui::Begin("Selected");
			m_selectedObject->DrawGui();
			
			ImGui::NewLine();
			ImGui::Text("Name");
			ImGui::InputText("#n", names[picked]);

			ImGui::NewLine();

			if (ImGui::Button("Delete"))
			{
				SceneObject::DeleteObject(sceneObjects[picked]);

				picked = -1;
				m_selectedObject = nullptr;
			}
			
			ImGui::End();
		}
	}
}
