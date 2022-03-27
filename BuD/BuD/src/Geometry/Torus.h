#pragma once

#include "Parameterized2DEntity.h"

namespace BuD
{
	class Torus : public Parameterized2DEntity
	{
	public:
		Torus(const DX11Device& device, float largeRadius, float smallRadius);

		virtual void DrawGui() override;
		virtual void UpdateRenderableModel() override;

		virtual GeometryType GetType() override { return GeometryType::TORUS; }

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
		float m_largeRadius, m_smallRadius;

		inline std::function<Vector3(Vector2)> TorusEquation()
		{
			return [this](Vector2 par) {
				return Vector3
				{
					(m_largeRadius + m_smallRadius * cosf(par.y)) * cosf(par.x),
					(m_largeRadius + m_smallRadius * cosf(par.y)) * sinf(par.x),
					m_smallRadius * sinf(par.y)
				};
			};
		}

		bool UpdateRadius(float largeRadius, float smallRadius);

		Vector3 m_color;

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;
	};
}
