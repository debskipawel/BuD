#pragma once

#include "Parameterized2DEntity.h"

namespace BuD
{
	class Torus : public Parameterized2DEntity
	{
	public:
		Torus(float largeRadius, float smallRadius)
			: Parameterized2DEntity({ 0.0f, 0.0f }, { 6.28318f, 6.28318f }, TorusEquation(largeRadius, smallRadius)),
			  m_largeRadius(largeRadius), m_smallRadius(smallRadius)
		{
			SampleBy({ 0.1963f, 0.1963f });
		}

		virtual void UpdateModel(const DX11Device& device) override;

	protected:
		float m_largeRadius, m_smallRadius;

		inline static std::function<Vector3(Vector2)> TorusEquation(float largeRadius, float smallRadius)
		{
			return [largeRadius, smallRadius](Vector2 par) {
				return Vector3
				{
					(largeRadius + smallRadius * cosf(par.y)) * cosf(par.x),
					(largeRadius + smallRadius * cosf(par.y)) * sinf(par.x),
					smallRadius * sinf(par.y)
				};
			};
		}

		static std::shared_ptr<DX11ConstantBuffer> ConstantBuffer(ID3D11Device* device);
		static std::shared_ptr<DX11ConstantBuffer> s_constantBuffer;
	};
}
