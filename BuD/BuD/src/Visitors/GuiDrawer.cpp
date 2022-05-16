#include "GuiDrawer.h"

#include <Objects/Independent/Point.h>
#include <Objects/Independent/Torus.h>

#include <Objects/PointBased/Curves/BezierCurve.h>
#include <Objects/PointBased/Curves/BezierCurveC0.h>
#include <Objects/PointBased/Curves/BezierCurveC2.h>
#include <Objects/PointBased/Curves/InterpolatedCurveC2.h>

#include <imgui.h>

namespace BuD
{
	GuiDrawer::GuiDrawer(Scene& scene)
		: m_scene(scene)
	{
	}

	void GuiDrawer::Action(Point& point)
	{
		auto position = point.Position();
		auto positionCopy = position;

		ImGui::Text("Translation");
		ImGui::DragFloat("t(x)", &position.x, 0.1f);
		ImGui::DragFloat("t(y)", &position.y, 0.1f);
		ImGui::DragFloat("t(z)", &position.z, 0.1f);

		if ((position - positionCopy).LengthSquared())
		{
			point.MoveBy(position - positionCopy, true);
		}
	}

	void GuiDrawer::Action(Torus& torus)
	{
		bool wasChanged = false;

		Vector3 currPos = torus.Position();
		Vector3 currPosCopy = currPos;

		ImGui::Text("Translation");
		ImGui::DragFloat("t(x)", &currPos.x, 0.5f);
		ImGui::DragFloat("t(y)", &currPos.y, 0.5f);
		ImGui::DragFloat("t(z)", &currPos.z, 0.5f);
		ImGui::NewLine();

		if ((currPos - currPosCopy).LengthSquared())
		{
			torus.MoveTo(currPos);
			wasChanged = true;
		}

		Vector3 currRot = torus.GetMesh(0)->m_rotation;
		Vector3 currRotCopy = currRot;

		ImGui::Text("Rotation");
		ImGui::DragFloat("r(x)", &currRot.x, 1.0f);
		ImGui::DragFloat("r(y)", &currRot.y, 1.0f);
		ImGui::DragFloat("r(z)", &currRot.z, 1.0f);

		if ((currRot - currRotCopy).LengthSquared())
		{
			torus.RotateTo(currRot);
			wasChanged = true;
		}

		ImGui::NewLine();

		Vector3 currScale = torus.GetMesh(0)->m_scale;
		Vector3 currScaleCopy = currRot;

		ImGui::Text("Scale");
		ImGui::DragFloat("s(x)", &currScale.x, 0.1f);
		ImGui::DragFloat("s(y)", &currScale.y, 0.1f);
		ImGui::DragFloat("s(z)", &currScale.z, 0.1f);
		ImGui::NewLine();

		if ((currScale - currScaleCopy).LengthSquared())
		{
			torus.ScaleTo(currScale);
			wasChanged = true;
		}

		int samplesU = torus.m_samplesU;
		int samplesV = torus.m_samplesV;

		const int samplesMinU = 3;
		const int samplesMinV = 3;

		ImGui::Text("Samples count");
		ImGui::DragInt("U", &samplesU, 1.0f, 1, 100);
		ImGui::DragInt("V", &samplesV, 1.0f, 1, 100);

		samplesU = samplesU < samplesMinU ? samplesMinU : samplesU;
		samplesV = samplesV < samplesMinV ? samplesMinV : samplesV;

		if (samplesU != torus.m_samplesU || samplesV != torus.m_samplesV)
		{
			wasChanged = true;
		}

		torus.m_samplesU = samplesU;
		torus.m_samplesV = samplesV;

		float r = torus.m_smallRadius;
		float R = torus.m_largeRadius;

		const float minSmallRadius = 0.001f;
		const float minLargeRadius = 0.001f;
		const float maxSmallRadius = torus.m_largeRadius;

		ImGui::Text("Torus radius");
		ImGui::DragFloat("Large", &R, 0.1f, 0.001f);
		ImGui::DragFloat("Small", &r, 0.1f, 0.001f, R);

		R = R < minLargeRadius ? minLargeRadius : R;
		r = r < minSmallRadius ? minSmallRadius : r;
		r = r > maxSmallRadius ? maxSmallRadius : r;

		if (torus.m_smallRadius != r || torus.m_largeRadius != R)
		{
			wasChanged = true;
		}

		torus.m_smallRadius = r;
		torus.m_largeRadius = R;

		if (wasChanged)
		{
			torus.OnUpdate();
		}
	}

	void GuiDrawer::Action(BezierCurveC0& curve)
	{
		DrawGui(curve);

		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &curve.m_drawPolygon);
	}

	void GuiDrawer::Action(BezierCurveC2& curve)
	{
		bool wasChanged = false;

		if (curve.m_inBernstein)
		{
			int i = 0;

			ImGui::Text("Bezier points");

			for (auto& point : curve.m_bernsteinPoints)
			{
				std::string name = "Bezier point " + std::to_string(i++) + " ##" + std::to_string(i);

				if (ImGui::TreeNode(name.c_str()))
				{
					Vector3 pointCopy = point;

					ImGui::Text("Position");
					ImGui::DragFloat("x", &point.x, 0.1f);
					ImGui::DragFloat("y", &point.y, 0.1f);
					ImGui::DragFloat("z", &point.z, 0.1f);

					if ((pointCopy - point).LengthSquared())
					{
						curve.ModifyBernsteinPoints(i - 1);
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}
		}
		else
		{
			DrawGui(curve);
		}

		ImGui::Checkbox("Bernstein base", &curve.m_inBernstein);
		ImGui::NewLine();

		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &curve.m_drawPolygon);

		bool deBoorCopy = curve.m_drawDeBoorPolygon;
		ImGui::Text("Draw De Boor polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##dbp", &curve.m_drawDeBoorPolygon);

		if (curve.m_drawDeBoorPolygon ^ deBoorCopy)
		{
			if (curve.m_drawDeBoorPolygon)
			{
				curve.m_meshes.push_back(curve.m_deBoorPolygonMesh);
			}
			else
			{
				curve.m_meshes.erase(std::find(curve.m_meshes.begin(), curve.m_meshes.end(), curve.m_deBoorPolygonMesh));
			}
		}
	}

	void GuiDrawer::Action(InterpolatedCurveC2& curve)
	{
		DrawGui(curve);

		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &curve.m_drawPolygon);
	}

	bool GuiDrawer::DrawGui(BezierCurve& curve)
	{
		bool wasChanged = false;

		int controlPointId = 0;

		ImGui::Text("Control points");

		for (auto& controlPoint : curve.m_controlPoints)
		{
			std::string name = "Point " + std::to_string(controlPoint->Id()) + " ##" + std::to_string(controlPointId++);

			if (ImGui::TreeNode(name.c_str()))
			{
				// gui for point
				Action(*controlPoint);

				auto removeName = "Remove CP " + std::to_string(controlPoint->Id());

				if (ImGui::Button(removeName.c_str()))
				{
					curve.RemoveControlPoint(controlPointId - 1);
					wasChanged = true;
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Add control points"))
		{
			auto objects = m_scene.GetSceneObjectsFiltered(ObjectType::POINT);

			for (auto& [id, obj] : objects)
			{
				auto point = reinterpret_cast<Point*>(obj.get());

				std::string name = "Add " + obj->Name() + std::to_string(id);
				bool selected = false;
				ImGui::Selectable(name.c_str(), &selected);

				if (selected)
				{
					curve.AddControlPoint(point);
					wasChanged = true;
				}
			}
		}

		return wasChanged;
	}
}