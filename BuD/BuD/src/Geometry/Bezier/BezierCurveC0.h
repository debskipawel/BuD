#pragma once

#include "BezierCurve.h"

namespace BuD
{
	class BezierCurveC0 : public BezierCurve
	{
	public:
		BezierCurveC0(const DX11Device& device, std::vector<SceneObject*> controlPoints);

		virtual GeometryType GetType() override;
	};
}
