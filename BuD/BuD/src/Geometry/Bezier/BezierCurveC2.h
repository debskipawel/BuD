#pragma once

#include "BezierCurve.h"

namespace BuD
{
	class BezierCurveC2 : public BezierCurve
	{
	public:
		BezierCurveC2(const DX11Device& device, std::vector<SceneObject*> controlPoints);

		virtual void AddControlPoint(SceneObject* obj) override;
		virtual void RemoveControlPoint(SceneObject* obj) override;

		virtual GeometryType GetType() override;
		virtual bool DrawGui() override;

	protected:

		void UpdateBezierPoints(int modifiedIndex);
		void CalculateBernsteinPoints();
		bool DrawGuiForEditingBezierPoints();

		bool m_inBernstein = false;

		std::vector<Vector3> m_bernsteinPoints;

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> GSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_gsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;
	};
}
