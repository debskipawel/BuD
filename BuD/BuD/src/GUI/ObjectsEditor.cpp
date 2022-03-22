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
			torus->GetMesh()->m_position = m_cursorPosition;

			m_objects.push_back(torus);
		}

		if (ImGui::Button("Add point"))
		{
			m_objects.push_back(
				std::make_shared<Point>(m_cursorPosition, device)
			);
		}

		ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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
					SelectionChanged();
					copySelected ? object->Select() : object->Unselect();
				}
			}

			ImGui::EndListBox();
		}

		ImGui::End();

		auto& selected = SceneObject::GetSelected();

		if (selected.Count() == 0)
		{
			return;
		}

		ImGui::Begin("Selected");

		if (selected.Count() == 1)
		{
			// draw GUI for a single object
			auto object = selected.Objects()[0];

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
		}
		else
		{
			Vector3 currPosition = m_beginPosition;
			Vector3 currRotation = m_beginRotation;
			Vector3 currScale = m_beginScale;

			// draw GUI for many objects
			ImGui::Text("Translation");
			ImGui::DragFloat("t(x)", &m_beginPosition.x, 0.1f);
			ImGui::DragFloat("t(y)", &m_beginPosition.y, 0.1f);
			ImGui::DragFloat("t(z)", &m_beginPosition.z, 0.1f);

			ImGui::Text("Rotation");
			ImGui::DragFloat("r(x)", &m_beginRotation.x, 1.0f);
			ImGui::DragFloat("r(y)", &m_beginRotation.y, 1.0f);
			ImGui::DragFloat("r(z)", &m_beginRotation.z, 1.0f);
			
			ImGui::Text("Scale");
			ImGui::DragFloat("s(x)", &m_beginScale.x, 0.1f);
			ImGui::DragFloat("s(y)", &m_beginScale.y, 0.1f);
			ImGui::DragFloat("s(z)", &m_beginScale.z, 0.1f);

			Vector3 translate = m_beginPosition - currPosition;
			Vector3 rotate = m_beginRotation - currRotation;
			Vector3 scale = m_beginScale / currScale;

			if (translate != Vector3{ 0.0f })
				SceneObject::GetSelected().MoveAll(translate);
			
			if (rotate != Vector3{ 0.0f })
				SceneObject::GetSelected().RotateAroundCentroid(rotate);

			if (scale != Vector3{ 1.0f })
				SceneObject::GetSelected().ScaleAroundCentroid(scale);
		}

		ImGui::End();
	}
	
	void ObjectsEditor::SelectionChanged()
	{
		m_beginPosition = { 0.0f, 0.0f, 0.0f };
		m_beginRotation = { 0.0f, 0.0f, 0.0f };
		m_beginScale = { 1.0f, 1.0f, 1.0f };
	}
}
