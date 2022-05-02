#pragma once

#include "BezierCurve.h"

namespace BuD
{
	class BezierCurveC2 : public BezierCurve
	{
	public:
		BezierCurveC2(const DX11Device& device, std::vector<SceneObject*> controlPoints);

		virtual void AddControlPoint(SceneObject* obj) override;
		virtual void RemoveControlPoint(int index) override;

		virtual std::vector<Vector3> VirtualControlPoints() override;

		virtual GeometryType GetType() override;
		virtual bool DrawGui() override;

	protected:

		void UpdateBezierPoints(int modifiedIndex);
		void CalculateBernsteinPoints();
		bool DrawGuiForEditingBezierPoints();

		bool m_inBernstein = false;
		bool m_drawDeBoorPolygon = false;

		std::vector<Vector3> m_bernsteinPoints;
		std::shared_ptr<Mesh> m_deBoorPolygonMesh;
	};
}
