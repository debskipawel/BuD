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
		Vector3 EquationResult(int i);

		std::vector<std::array<Vector3, 4>> m_splineParameters;

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> GSConstantBuffer1(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> GSConstantBuffer2(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_gsConstantBuffer1;
		static std::shared_ptr<DX11ConstantBuffer> s_gsConstantBuffer2;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;

		virtual GeometryType GetType() override;
	};
}
