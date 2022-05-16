#include "ObjectsEditor.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "Camera/CameraFactory.h"

#include <Objects/Abstract/PointBasedObject.h>
#include <Objects/SceneObjectGroup.h>

#include <algorithm>
#include <iterator>

namespace BuD
{
	ObjectsEditor::ObjectsEditor(Scene& scene, std::shared_ptr<Win32Window> window)
		: m_scene(scene), m_window(window), m_guiDrawer(scene)
	{
		m_activeCamera = CameraFactory::MakePerspective(Vector3(0.0f, 0.0f, 3.0f), Vector3(0.0f, 0.0f, -1.0f));
		m_selectedType = RenderingMode::STANDARD;
	}

	std::shared_ptr<AbstractCamera> ObjectsEditor::GetCamera()
	{
		return m_activeCamera;
	}
	
	void ObjectsEditor::DrawGui(const DX11Device& device)
	{
		DrawMainSettings(device);
		DrawSelectableList(device);

		DrawSelectableConfig(device);
	}

	void ObjectsEditor::SetCursorTo(int pixelX, int pixelY)
	{
		m_cursorPosition = m_activeCamera->MoveWorldPointToPixels(m_cursorPosition, pixelX, pixelY);
	}
	
	void ObjectsEditor::SelectionChanged()
	{
		m_beginPosition = { 0.0f, 0.0f, 0.0f };
		m_beginRotation = { 0.0f, 0.0f, 0.0f };
		m_beginScale = { 1.0f, 1.0f, 1.0f };
	}

	static float patchWidth, patchLength;
	static int samplesU, samplesV, patchesU, patchesV;
	static Vector3 patchPosition;
	static bool asCylinder;
	
	void ObjectsEditor::DrawMainSettings(const DX11Device& device)
	{
		auto& io = ImGui::GetIO();
		ImGui::Begin("Main settings", nullptr, io.WantCaptureMouse);

		if (ImGui::TreeNode("Camera settings"))
		{
			if (ImGui::BeginCombo("##cameraType", "Camera type"))
			{
				RenderingMode types[] = { RenderingMode::STANDARD, RenderingMode::ANAGLIPH };
				std::string names[] = { "Perspective", "Anagliph" };

				for (int i = 0; i < 2; i++)
				{
					GuiForCamera(types[i], names[i]);
				}

				ImGui::EndCombo();
			}

			ImGui::NewLine();

			m_activeCamera->DrawGui();
			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::Text("Cursor position");
		ImGui::DragFloat("X", &m_cursorPosition.x, 0.1f);
		ImGui::DragFloat("Y", &m_cursorPosition.y, 0.1f);
		ImGui::DragFloat("Z", &m_cursorPosition.z, 0.1f);
		ImGui::NewLine();

		Vector4 cursorPosition = Vector4(m_cursorPosition.x, m_cursorPosition.y, m_cursorPosition.z, 1.0f);
		Vector4 screenPosition{};

		auto transformMatrix = m_activeCamera->GetViewMatrix() * m_activeCamera->GetProjectionMatrix();

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
			m_scene.CreateTorus(device, m_cursorPosition, 3.0f, 1.0f);
		}

		if (ImGui::Button("Add point"))
		{
			auto point = m_scene.CreatePoint(device, m_cursorPosition);

			auto selected = m_scene.GetAllSelected();

			for (auto& [id, obj] : selected)
			{
				if ((obj->GetFlags() & ObjectFlags::POINT_BASED) != ObjectFlags::NONE)
				{
					auto pointBased = reinterpret_cast<PointBasedObject*>(obj.get());
					pointBased->AddControlPoint(point.get());
				}
			}
		}

		if (ImGui::Button("Add C0 surface"))
		{
			patchWidth = patchLength = 1.0f;
			samplesU = samplesV = 3;
			patchesU = patchesV = 1;
			patchPosition = m_cursorPosition;
			asCylinder = false;

			ImGui::OpenPopup("new_surface");
		}

		if (ImGui::BeginPopupModal("new_surface"))
		{
			// TODO: gui for surface
			ImGui::Text("Patches size:");
			ImGui::DragFloat("width ##patchWidth", &patchWidth, 0.01f);
			ImGui::DragFloat("length ##patchLength", &patchLength, 0.01f);

			ImGui::Separator();

			ImGui::Text("Sample count");
			ImGui::DragInt("u ##uSamples", &samplesU, 1.0f, 1, 50);
			ImGui::DragInt("v ##vSamples", &samplesV, 1.0f, 1, 50);

			ImGui::Separator();

			ImGui::Text("Patch count");
			ImGui::DragInt("u ##uPatches", &patchesU, 1.0f, 1, 10);
			ImGui::DragInt("v ##vPatches", &patchesV, 1.0f, 1, 10);

			ImGui::Separator();

			ImGui::Text("Position");
			ImGui::DragFloat("X ##patchPosx", &patchPosition.x, 0.1f);
			ImGui::DragFloat("Y ##patchPosy", &patchPosition.y, 0.1f);
			ImGui::DragFloat("Z ##patchPosz", &patchPosition.z, 0.1f);

			ImGui::Separator();

			ImGui::Checkbox("As cylinder: ", &asCylinder);

			if (ImGui::Button("Add"))
			{
				m_scene.CreateBezierSurfaceC0(device, patchPosition, patchWidth, patchLength, patchesU, patchesV, samplesU, samplesV, asCylinder);

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		
		ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}
	
	void ObjectsEditor::DrawSelectableList(const DX11Device& device)
	{
		auto& io = ImGui::GetIO();
		auto objects = m_scene.GetAllSceneObjects();

		ImGui::Begin("Scene objects", nullptr, io.WantCaptureMouse);
		ImGui::Text("Objects");
		if (ImGui::BeginListBox("##ob"))
		{
			for (auto& [id, object] : objects)
			{
				bool selected = object->Selected();
				bool copySelected = selected;

				std::string name = std::to_string(id) + ": " + object->Name();

				auto res = ImGui::Selectable(name.c_str(), &copySelected);

				if (copySelected != selected)
				{
					SelectionChanged();
					copySelected ? object->OnSelect() : object->OnUnselect();
				}
			}

			ImGui::EndListBox();
		}

		ImGui::End();
	}
	
	void ObjectsEditor::DrawSelectableConfig(const DX11Device& device)
	{
		auto selected = m_scene.GetAllSelected();
		SceneObjectsGroup group(selected);

		if (selected.size() == 0)
		{
			return;
		}

		ImGui::Begin("Selected");

		if (selected.size() == 1)
		{
			// draw GUI for a single object
			auto object = (*selected.begin()).second;
			m_guiDrawer.Visit(*object.get());

			ImGui::NewLine();
			ImGui::Text("Name");

			auto name = object->Name();
			ImGui::InputText("##n", &name);

			ImGui::NewLine();

			if (ImGui::Button("Delete"))
			{
				m_scene.RemoveSceneObject(object);
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
			{
				group.MoveAll(translate);
			}

			if (rotate != Vector3{ 0.0f })
			{
				group.RotateAroundCentroid(rotate);
			}

			if (scale != Vector3{ 1.0f } && scale != Vector3{ 0.0f })
			{
				group.ScaleAroundCentroid(scale);
			}
		}

		if (group.GetType() == ObjectFlags::POINT)
		{
			ImGui::NewLine();

			if (ImGui::Button("Add Bezier C0"))
			{
				std::vector<Point*> result;
				std::transform(selected.begin(), selected.end(), std::inserter(result, result.end()),
					[](std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return reinterpret_cast<Point*>(record.second.get()); }
				);

				m_scene.CreateBezierCurveC0(device, result);
			}

			if (ImGui::Button("Add Bezier C2"))
			{
				std::vector<Point*> result;
				std::transform(selected.begin(), selected.end(), std::inserter(result, result.end()),
					[](std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return reinterpret_cast<Point*>(record.second.get()); }
				);

				m_scene.CreateBezierCurveC2(device, result);
			}

			if (ImGui::Button("Add interpolated C2"))
			{
				std::vector<Point*> result;
				std::transform(selected.begin(), selected.end(), std::inserter(result, result.end()),
					[](std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return reinterpret_cast<Point*>(record.second.get()); }
				);

				m_scene.CreateInterpolatedCurveC2(device, result);
			}
		}

		ImGui::End();
	}
	
	void ObjectsEditor::UpdateCamera(const RenderingMode& selectedType)
	{
		auto position = m_activeCamera->Position();
		auto direction = m_activeCamera->Front();
		auto worldUp = m_activeCamera->WorldUp();

		auto fov = m_activeCamera->Fov();
		auto ratio = m_activeCamera->Ratio();

		switch (selectedType)
		{
			case RenderingMode::STANDARD:
			{
				m_activeCamera = CameraFactory::MakePerspective(position, direction, worldUp, ratio);
				break;
			}
			case RenderingMode::ANAGLIPH:
			{
				m_activeCamera = CameraFactory::MakeStereoscopic(position, direction, worldUp, ratio);
				break;
			}
			default:
			{
				return;
			}
		}

		m_activeCamera->SetFov(fov);
		m_activeCamera->UpdateViewport(m_window->Width(), m_window->Height());
	}
	
	void ObjectsEditor::GuiForCamera(const RenderingMode& cameraType, std::string name)
	{
		bool selected = m_selectedType == cameraType;
		bool selectedCopy = selected;

		if (ImGui::Selectable(name.c_str(), &selected))
		{
			m_selectedType = cameraType;

			if (selected != selectedCopy)
			{
				UpdateCamera(cameraType);
			}
		}

		if (selected)
		{
			ImGui::SetItemDefaultFocus();
		}
	}
}
