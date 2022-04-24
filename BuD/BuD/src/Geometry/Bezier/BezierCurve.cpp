#include "BezierCurve.h"

#include <imgui.h>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace BuD
{
	BezierCurve::BezierCurve(std::vector<SceneObject*> controlPoints)
		: m_controlPoints(controlPoints), m_color(1.0f, 1.0f, 1.0f)
	{
	}

	bool BezierCurve::DrawGui()
	{
		bool wasChanged = false;

		wasChanged |= DrawGuiForEditingControlPoints();
		ImGui::NewLine();
		
		wasChanged |= DrawGuiForAddingControlPoints();
		ImGui::NewLine();
		
		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &m_drawPolygon);

		return wasChanged;
	}

	void BezierCurve::AddControlPoint(SceneObject* obj)
	{
		m_controlPoints.push_back(obj);
	}

	void BezierCurve::RemoveControlPoint(int index)
	{
		if (index >= 0 && index < m_controlPoints.size())
		{
			m_controlPoints.erase(m_controlPoints.begin() + index);
		}
	}
	
	void BezierCurve::FilterControlPoints()
	{
		std::vector<SceneObject*> filteredControlPoints;
		filteredControlPoints.reserve(m_controlPoints.size());

		std::copy_if(m_controlPoints.begin(), m_controlPoints.end(), std::back_inserter(filteredControlPoints), [](SceneObject* obj) { return !obj->ShouldBeDeleted(); });
		m_controlPoints = filteredControlPoints;
	}

	void BezierCurve::UpdateCentroid()
	{
		auto verticesSum = std::accumulate(m_controlPoints.begin(), m_controlPoints.end(), Vector3{}, [](Vector3 a, SceneObject* obj) { return a + obj->GetMesh(0)->m_position; });
		
		for (auto& mesh : m_meshes)
		{
			mesh->m_position = verticesSum / m_controlPoints.size();
		}
	}
	
	RECT BezierCurve::GetSurroundingRectangle(std::shared_ptr<AbstractCamera> camera, UINT width, UINT height)
	{
		long minX = width, maxX = 0;
		long minY = height, maxY = 0;

		auto matrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();

		for (auto& cp : m_controlPoints)
		{
			auto position = cp->GetMesh(0)->m_position;
			auto trPosition = Vector4{ position.x, position.y, position.z, 1.0f };
			trPosition = Vector4::Transform(trPosition, matrix);
			trPosition /= trPosition.w;

			long xPos = width * (std::clamp(trPosition.x, -1.0f, 1.0f) + 1.0f) / 2.0f;
			long yPos = height * (std::clamp(trPosition.y, -1.0f, 1.0f) + 1.0f) / 2.0f;

			maxX = max(xPos, maxX);
			minX = min(xPos, minX);
			maxY = max(yPos, maxY);
			minY = min(yPos, minY);
		}

		return { minX, minY, maxX, maxY };
	}

	bool BezierCurve::DrawGuiForAddingControlPoints()
	{
		bool wasChanged = false;

		if (ImGui::CollapsingHeader("Add control points"))
		{
			auto& objects = SceneObject::GetAll();

			for (auto& [id, obj] : objects)
			{
				if (obj->GetType() == GeometryType::POINT)
				{
					std::string name = "Add " + *obj->Name() + std::to_string(id);
					bool selected = false;
					ImGui::Selectable(name.c_str(), &selected);

					if (selected)
					{
						AddControlPoint(obj);
						wasChanged = true;
					}
				}
			}
		}

		return wasChanged;
	}
	
	bool BezierCurve::DrawGuiForEditingControlPoints()
	{
		bool wasChanged = false;
		int controlPointId = 0;

		ImGui::Text("Control points");

		for (auto& controlPoint : m_controlPoints)
		{
			std::string name = "Point " + std::to_string(controlPoint->Id()) + " ##" + std::to_string(controlPointId++);

			if (ImGui::TreeNode(name.c_str()))
			{
				if (controlPoint->DrawGui())
				{
					wasChanged = true;
				}

				auto removeName = "Remove CP " + std::to_string(controlPoint->Id());

				if (ImGui::Button(removeName.c_str()))
				{
					RemoveControlPoint(controlPointId - 1);
					wasChanged = true;
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		return wasChanged;
	}
}