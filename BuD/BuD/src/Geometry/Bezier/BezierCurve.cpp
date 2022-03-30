#include "BezierCurve.h"

#include <imgui.h>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace BuD
{
	void BezierCurve::DrawGui()
	{
		auto& objects = SceneObject::GetAll();

		ImGui::Text("Control points");
		for (auto& controlPoint : m_controlPoints)
		{
			std::string name = "Point " + std::to_string(controlPoint->Id());

			if (ImGui::TreeNode(name.c_str()))
			{
				controlPoint->DrawGui();

				auto removeName = "Remove CP " + std::to_string(controlPoint->Id());

				if (ImGui::Button(removeName.c_str()))
				{
					RemoveControlPoint(controlPoint);
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		ImGui::NewLine();

		if (ImGui::CollapsingHeader("Add control points"))
		{
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
					}
				}
			}
		}

		ImGui::NewLine();
		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &m_drawPolygon);
	}

	void BezierCurve::AddControlPoint(SceneObject* obj)
	{
		m_controlPoints.push_back(obj);
	}

	void BezierCurve::RemoveControlPoint(SceneObject* obj)
	{
		auto res = std::find(m_controlPoints.begin(), m_controlPoints.end(), obj);

		if (res != m_controlPoints.end())
		{
			m_controlPoints.erase(res);
		}
	}
	
	BezierCurve::BezierCurve(std::vector<SceneObject*> controlPoints)
		: m_controlPoints(controlPoints), m_color(1.0f, 1.0f, 1.0f)
	{
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
}