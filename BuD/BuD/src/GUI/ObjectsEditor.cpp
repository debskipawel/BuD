#include "ObjectsEditor.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include <ImGuiFileDialog.h>

#include "Camera/CameraFactory.h"

#include <Algorithms/HoleFindingAlgorithm.h>

#include <Objects/Abstract/PointBasedObject.h>
#include <Objects/SceneObjectGroup.h>

#include <algorithm>
#include <iterator>

namespace BuD
{
	ObjectsEditor::ObjectsEditor(std::shared_ptr<Win32Window> window)
		: m_scene(), m_window(window), m_guiDrawer(m_scene)
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
		DrawContextMenu(device);

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

	const int minSamples = 3, maxSamples = 50;
	const int minPatches = 1, maxPatches = 10;
	const float minWidth = 0.01f;
	
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
			samplesU = samplesV = 4;
			patchesU = patchesV = 1;
			patchPosition = m_cursorPosition;
			asCylinder = false;

			ImGui::OpenPopup("Add C0 surface ##new_surface");
		}

		if (ImGui::Button("Add C2 surface"))
		{
			patchWidth = patchLength = 1.0f;
			samplesU = samplesV = 4;
			patchesU = patchesV = 1;
			patchPosition = m_cursorPosition;
			asCylinder = false;

			ImGui::OpenPopup("Add C2 surface ##new_surface");
		}

		if (DrawSurfacePopup("Add C0 surface ##new_surface"))
		{
			m_scene.CreateBezierSurfaceC0(device, patchPosition, patchWidth, patchLength, patchesU, patchesV, samplesU, samplesV, asCylinder);
		}

		if (DrawSurfacePopup("Add C2 surface ##new_surface"))
		{
			m_scene.CreateBezierSurfaceC2(device, patchPosition, patchWidth, patchLength, patchesU, patchesV, samplesU, samplesV, asCylinder);
		}
		
		ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	void ObjectsEditor::DrawContextMenu(const DX11Device& device)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save to file..."))
				{
					ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlg", "Save to file", ".json", ".");
				}

				if (ImGui::MenuItem("Read from file..."))
				{
					ImGuiFileDialog::Instance()->OpenDialog("OpenFileDlg", "Choose file", ".json", ".");
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGuiFileDialog::Instance()->Display("SaveFileDlg"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
				
				try
				{
					m_scene.SaveToFile(filePathName);
				}
				catch (std::exception e)
				{
					std::string message = e.what();
					std::wstring wMessage = std::wstring(message.begin(), message.end());

					wMessage = L"Error occured while writing to the file: " + wMessage;

					MessageBox(nullptr, wMessage.c_str(), L"Error", 0);
				}
			}

			ImGuiFileDialog::Instance()->Close();
		}

		if (ImGuiFileDialog::Instance()->Display("OpenFileDlg"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
				std::wstring wFilePathName(filePathName.begin(), filePathName.end());
				std::wstring wFilePath(filePath.begin(), filePath.end());

				try
				{
					m_scene = Scene();
					Scene::ReadFromFile(device, m_scene, filePathName);
				}
				catch (std::exception e)
				{
					std::string message = e.what();
					std::wstring wMessage = std::wstring(message.begin(), message.end());

					wMessage = L"Error occured while reading the file: " + wMessage;

					MessageBox(nullptr, wMessage.c_str(), L"Error", 0);
				}
			}

			ImGuiFileDialog::Instance()->Close();
		}
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

				std::string name = std::to_string(id) + ": " + object->GetName();

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

			auto name = object->GetName();
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

			auto& objects = group.Objects();
			auto centroid = group.Centroid();
			auto element = std::max_element(objects.begin(), objects.end(),
				[centroid](std::pair<uint32_t, std::shared_ptr<SceneObject>> first, std::pair<uint32_t, std::shared_ptr<SceneObject>> second)
				{
					return (first.second->Position() - centroid).LengthSquared() > (second.second->Position() - centroid).LengthSquared();
				}
			);

			float maxDist = (element->second->Position() - centroid).Length();

			if (maxDist <= 0.2f && objects.size() > 1)
			{
				if (ImGui::Button("Merge"))
				{
					auto last = reinterpret_cast<Point*>(objects.rbegin()->second.get());

					for (auto& [id, obj] : objects)
					{
						auto point = reinterpret_cast<Point*>(obj.get());

						if (point == last || m_scene.GetSceneObject(id) == nullptr)
						{
							break;
						}

						auto newPoint = Point::Merge(device, point, last);
						last = newPoint.get();
					}
				}
			}

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
		else if (group.GetType() == ObjectFlags::BEZIER_PATCH_C0)
		{
			auto& objects = group.Objects();
			std::vector<BezierPatchC0*> patches;
			
			std::transform(objects.begin(), objects.end(), std::back_inserter(patches),
				[](std::pair<uint32_t, std::shared_ptr<SceneObject>> pair)
				{
					return reinterpret_cast<BezierPatchC0*>(pair.second.get());
				}
			);

			auto [firstLine, secondLine] = HoleFindingAlgorithm::FindHole(patches);

			if (firstLine.size() > 0)
			{
				if (ImGui::Button("Mark Cycle"))
				{
					for (auto& p : firstLine)
					{
						p->OnSelect();
					}
				}
			}
		}

		ImGui::End();
	}

	bool ObjectsEditor::DrawSurfacePopup(std::string name)
	{
		if (ImGui::BeginPopupModal(name.c_str()))
		{
			// TODO: gui for surface
			ImGui::Text("Patches size:");
			ImGui::DragFloat(asCylinder ? "radius ##patchWidth" : "width ##patchWidth", &patchWidth, minWidth);
			ImGui::DragFloat(asCylinder ? "height ##patchWidth" : "length ##patchLength", &patchLength, minWidth);

			patchWidth = max(patchWidth, minWidth);
			patchLength = max(patchLength, minWidth);

			ImGui::Separator();

			ImGui::Text("Sample count");
			ImGui::DragInt("u ##uSamples", &samplesU, 0.5f, asCylinder ? minSamples + 1 : minSamples, maxSamples);
			ImGui::DragInt("v ##vSamples", &samplesV, 0.5f, asCylinder ? minSamples + 1 : minSamples, maxSamples);

			samplesU = std::clamp(samplesU, minSamples, maxSamples);
			samplesV = std::clamp(samplesV, minSamples, maxSamples);

			ImGui::Separator();

			ImGui::Text("Patch count");
			ImGui::DragInt("u ##uPatches", &patchesU, 0.5f, asCylinder ? minPatches + 1 : minPatches, maxPatches);
			ImGui::DragInt("v ##vPatches", &patchesV, minPatches, maxPatches);

			patchesU = std::clamp(patchesU, asCylinder ? minPatches + 1 : minPatches, maxPatches);
			patchesV = std::clamp(patchesV, minPatches, maxPatches);

			ImGui::Separator();

			ImGui::Text("Position");
			ImGui::DragFloat("X ##patchPosx", &patchPosition.x, 0.1f);
			ImGui::DragFloat("Y ##patchPosy", &patchPosition.y, 0.1f);
			ImGui::DragFloat("Z ##patchPosz", &patchPosition.z, 0.1f);

			ImGui::Separator();

			ImGui::Checkbox("As cylinder: ", &asCylinder);

			if (ImGui::Button("Add"))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();

				return true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return false;
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
