#pragma once

#include "ParameterizedObject.h"

namespace BuD
{
	class Parameterized2DEntity : public ParameterizedObject<Vector2>
	{
	public:
		Parameterized2DEntity(Vector2 minDomain, Vector2 maxDomain, std::function<Vector3(Vector2)> objectFunction);

		virtual void SampleBy(Vector2 interval) override;

		virtual void UpdateModel(const DX11Device& device) = 0;

	protected:
		Vector2 m_prevInterval;

		std::vector<Vector3> m_vertices;
		std::vector<unsigned short> m_indices;
	};
}
