#pragma once

#include "BezierCurve.h"

namespace BuD
{
	class BezierCurveC2 : public BezierCurve
	{
	public:
		BezierCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints);

		virtual void Accept(AbstractVisitor& visitor) override;
		inline virtual ObjectType GetType() { return ObjectType::BEZIER_CURVE_C2; }

		virtual void OnUpdate() override;

	protected:
		virtual void ModifyBernsteinPoints(int modifiedIndex);

		bool m_inBernstein = false;
		bool m_drawDeBoorPolygon = false;

		std::vector<Vector3> m_bernsteinPoints;
		std::shared_ptr<Mesh> m_deBoorPolygonMesh;

		friend class GuiDrawer;
	};
}
