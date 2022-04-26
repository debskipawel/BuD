#pragma once

#include "BezierCurve.h"

#include <array>

namespace BuD
{
	class InterpolatedBezierCurveC2 : public BezierCurve
	{
	public:
		InterpolatedBezierCurveC2(const DX11Device& device, std::vector<SceneObject*> controlPoints);

	protected:
		std::vector<Vector3> SolveTridiagonal();
		void CalculateSplineParameters();
		void TransformPowerToBernstein();
		Vector3 EquationResult(int i);

		virtual RECT GetSurroundingRectangle(std::shared_ptr<AbstractCamera> camera, UINT width, UINT height) override;

		std::vector<std::array<Vector3, 4>> m_splineParameters;
		std::vector<float> m_distances;

		virtual GeometryType GetType() override;
	};
}
