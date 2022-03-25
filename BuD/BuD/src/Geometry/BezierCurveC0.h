#pragma once

#include "SceneObject.h"

namespace BuD
{
	class BezierCurveC0 : public SceneObject
	{
	public:
		BezierCurveC0(const DX11Device& device);

		virtual void DrawGui() override { }

		virtual void Select() override
		{
			SceneObject::Select();
			m_color = { 0.7f, 0.5f, 0.0f };
		}

		virtual void Unselect()
		{
			SceneObject::Unselect();
			m_color = { 1.0f, 1.0f, 1.0f };
		}

	protected:
		Vector3 m_color;

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;
	};
}
