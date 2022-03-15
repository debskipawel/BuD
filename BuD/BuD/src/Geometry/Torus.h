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

		static std::shared_ptr<DX11ConstantBuffer> ConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_constantBuffer;
	};
}
