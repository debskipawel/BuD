#include "ObjectsEditor.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "Geometry/Point.h"
#include "Geometry/Torus.h"
#include "Geometry/Bezier/BezierCurve.h"
#include "Geometry/Bezier/BezierCurveC0.h"
#include "Geometry/Bezier/BezierCurveC2.h"
#include "Geometry/Bezier/InterpolatedBezierCurveC2.h"

#include <algorithm>

namespace BuD
{
	ObjectsEditor::ObjectsEditor(std::vector<std::shared_ptr<SceneObject>>& objects, std::shared_ptr<AbstractCamera> camera, std::shared_ptr<Win32Window> window)
		: m_objects(objects), m_camera(camera), m_window(window)
	{
	}
	
	void ObjectsEditor::DrawGui(const DX11Device& device)
	{
		DrawMainSettings(device);
		DrawSelectableList(device);

		DrawSelectableConfig(device);
	}

	void ObjectsEditor::SetCursorTo(int pixelX, int pixelY)
	{
		m_cursorPosition = m_camera->MoveWorldPointToPixels(m_cursorPosition, pixelX, pixelY);
	}
	
	void ObjectsEditor::SelectionChanged()
	{
		m_beginPosition = { 0.0f, 0.0f, 0.0f };
		m_beginRotation = { 0.0f, 0.0f, 0.0f };
		m_beginScale = { 1.0f, 1.0f, 1.0f };
	}
	
	void ObjectsEditor::DrawMainSettings(const DX11Device& device)
	{
		auto& io = ImGui::GetIO();
		ImGui::Begin("Main settings", nullptr, io.WantCaptureMouse);

		if (ImGui::TreeNode("Camera settings"))
		{
			m_camera->DrawGui();
			ImGui::TreePop();
		}

		ImGui::Text("Cursor position");
		ImGui::DragFloat("X", &m_cursorPosition.x, 0.1f);
		ImGui::DragFloat("Y", &m_cursorPosition.y, 0.1f);
		ImGui::DragFloat("Z", &m_cursorPosition.z, 0.1f);
		ImGui::NewLine();

		Vector4 cursorPosition = Vector4(m_cursorPosition.x, m_cursorPosition.y, m_cursorPosition.z, 1.0f);
		Vector4 screenPosition{};

		auto transformMatrix = m_camera->GetViewMatrix() * m_camera->GetProjectionMatrix();

		Vector4::Transform(cursorPosition, transformMatrix, screenPosition);
		screenPosition /= screenPosition.w;

		int pixelX = 0.5f * (screenPosition.x + 1.0f) * m_window->Width();
		int pixelY = 0.5f * (screenPosition.y + 1.0f) * m_window->Height();

		if (pixelX >= 0 && pixelX <= m_window->Width() && pixelY >= 0 && pixelY <= m_window->Height())
		{
			ImGui::Text("Screen coords");
			ImGui::Text("x: %d, y: %d", pixelX, pixelY);
			ImGui::NewLine();
		}

		if (ImGui::Button("Add torus"))
		{
			auto torus = std::make_shared<Torus>(device, 3.0f, 1.0f);
			torus->MoveTo(m_cursorPosition);

			m_objects.push_back(torus);
		}

		if (ImGui::Button("Add point"))
		{
			auto point = std::make_shared<Point>(m_cursorPosition, device);

			m_objects.push_back(point);

			auto& selected = SceneObject::GetSelected();

			for (auto& obj : selected.Objects())
			{
				if (static_cast<int>(selected.GetType()) & static_cast<int>(GeometryType::BEZIER))
				{
					reinterpret_cast<BezierCurve*>(obj)->AddControlPoint(point.get());
				}
			}
		}
		
		ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}
	
	void ObjectsEditor::DrawSelectableList(const DX11Device& device)
	{
		auto& io = ImGui::GetIO();
		auto& objects = SceneObject::GetAll();

		ImGui::Begin("Scene objects", nullptr, io.WantCaptureMouse);
		ImGui::Text("Objects");
		if (ImGui::BeginListBox("##ob"))
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
	}
	
	void ObjectsEditor::DrawSelectableConfig(const DX11Device& device)
	{
		auto& selected = SceneObject::GetSelected();

		if (selected.Count() == 0)
		{
			return;
		}

		ImGui::Begin("Selected");

		if (selected.Count() == 1)
		{
			// draw GUI for a single object
			auto object = *selected.Objects().begin();
			object->DrawGui();

			ImGui::NewLine();
			ImGui::Text("Name");
			ImGui::InputText("##n", object->Name());

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

			constexpr float minScale = 0.01f;

			ImGui::Text("Scale");
			ImGui::DragFloat("s(x)", &m_beginScale.x, 0.1f);
			ImGui::DragFloat("s(y)", &m_beginScale.y, 0.1f);
			ImGui::DragFloat("s(z)", &m_beginScale.z, 0.1f);

			Vector3 translate = m_beginPosition - currPosition;
			Vector3 rotate = m_beginRotation - currRotation;

			m_beginScale = Vector3
			{
				m_beginScale.x < minScale ? minScale : m_beginScale.x,
				m_beginScale.y < minScale ? minScale : m_beginScale.y,
				m_beginScale.z < minScale ? minScale : m_beginScale.z,
			};

			Vector3 scale = m_beginScale / currScale;

			if (translate != Vector3{ 0.0f })
				selected.MoveAll(translate);

			if (rotate != Vector3{ 0.0f })
				selected.RotateAroundCentroid(rotate);

			if (scale != Vector3{ 1.0f } && scale != Vector3{ 0.0f })
				selected.ScaleAroundCentroid(scale);
		}

		if (selected.GetType() == GeometryType::POINT)
		{
			ImGui::NewLine();

			if (ImGui::Button("Add Bezier C0"))
			{
				auto bezier = std::make_shared<BezierCurveC0>(device, selected.Objects());

				m_objects.push_back(bezier);
			}

			if (ImGui::Button("Add Bezier C2"))
			{
				auto bezier = std::make_shared<BezierCurveC2>(device, selected.Objects());

				m_objects.push_back(bezier);
			}

			if (ImGui::Button("Add interpolated C2"))
			{
				auto bezier = std::make_shared<InterpolatedBezierCurveC2>(device, selected.Objects());

				m_objects.push_back(bezier);
			}
		}

		ImGui::End();
	}
}
