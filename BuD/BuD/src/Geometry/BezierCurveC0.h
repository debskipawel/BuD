#pragma once

#include "SceneObject.h"

#include "Point.h"

namespace BuD
{
	class BezierCurveC0 : public SceneObject
	{
	public:
		BezierCurveC0(const DX11Device& device, std::vector<std::shared_ptr<Point>> controlPoints);

		virtual void DrawGui() override { }

		virtual void Select() override
		{
			SceneObject::Select();

			for (auto& point : m_controlPoints)
			{
				point->Select();
			}

			m_color = { 0.7f, 0.5f, 0.0f };
		}

		virtual void Unselect()
		{
			SceneObject::Unselect();

			for (auto& point : m_controlPoints)
			{
				point->Unselect();
			}

			m_color = { 1.0f, 1.0f, 1.0f };
		}

	protected:
		Vector3 m_color;

		std::vector<std::shared_ptr<Point>> m_controlPoints;

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> GSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_gsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;
	};
}
