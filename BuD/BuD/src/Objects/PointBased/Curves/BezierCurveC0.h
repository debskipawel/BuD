#pragma once

#include "BezierCurve.h"

namespace BuD
{
	class BezierCurveC0 : public BezierCurve
	{
	public:
		BezierCurveC0(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints);

		virtual void Accept(AbstractVisitor& visitor) override;
		virtual void OnUpdate() override;

		inline virtual ObjectFlags GetFlags() override { return ObjectFlags::BEZIER_CURVE_C0; }

		friend class ObjectSerializer;
	};
}
