#pragma once

#include "BezierCurve.h"

namespace BuD
{
	class BezierCurveC0 : public BezierCurve
	{
	public:
		BezierCurveC0(const DX11Device& device, std::vector<SceneObject*> controlPoints);

		virtual GeometryType GetType() override;

	protected:

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> GSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_gsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;
	};
}
