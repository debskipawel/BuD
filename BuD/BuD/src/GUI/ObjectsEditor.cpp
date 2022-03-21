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

		auto& objects = SceneObject::GetAll();

		ImGui::Begin("Scene objects");
		if (ImGui::BeginListBox("Objects"))
		{
			for (auto& [id, object] : objects)
			{
				bool selected = object->IsSelected();
				bool copySelected = selected;

				std::string name = std::to_string(id) + ": " + *object->Name();

				auto res = ImGui::Selectable(name.c_str(), &copySelected);

				if (copySelected != selected)
				{
					copySelected ? object->Select() : object->Unselect();
				}
			}

			ImGui::EndListBox();
		}

		ImGui::End();

		auto& selected = SceneObject::GetSelected();
		
		if (selected.Count() == 1)
		{
			// draw GUI for a single object
			auto object = selected.Objects()[0];

			ImGui::Begin("Selected");

			// TODO: update centroid on position modification
			object->DrawGui();
			
			ImGui::NewLine();
			ImGui::Text("Name");
			ImGui::InputText("#n", object->Name());

			ImGui::NewLine();

			if (ImGui::Button("Delete"))
			{
				object->Unselect();
				SceneObject::DeleteObject(object->Id());
			}

			ImGui::End();
		}
		else
		{
			// draw GUI for many objects
		}
	}
}
