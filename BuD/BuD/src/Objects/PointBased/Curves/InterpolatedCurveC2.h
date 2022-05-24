#pragma once

#include "BezierCurve.h"

#include <array>

namespace BuD
{
	class InterpolatedCurveC2 : public BezierCurve
	{
	public:
		InterpolatedCurveC2(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints);

		virtual void Accept(AbstractVisitor& visitor) override;
		virtual void OnUpdate() override;

	protected:
		std::vector<std::array<Vector3, 4>> m_splineParameters;
		std::vector<float> m_distances;

	private:
		std::vector<Vector3> SolveTridiagonal();
		void TransformPowerToBernstein();

		Vector3 EquationResult(int i);

		friend class ObjectSerializer;
	};
}
