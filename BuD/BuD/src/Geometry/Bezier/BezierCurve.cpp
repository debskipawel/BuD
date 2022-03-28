#include "BezierCurve.h"

#include <imgui.h>

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
}