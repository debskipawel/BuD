#pragma once

#include "ParameterizedObject.h"

namespace BuD
{
	class Parameterized2DEntity : public ParameterizedObject<Vector2>
	{
	public:
		Parameterized2DEntity(Vector2 minDomain, Vector2 maxDomain, std::function<Vector3(Vector2)> objectFunction);

		virtual void UpdateRenderableModel() = 0;
		virtual void DrawGui() override;

	protected:
		virtual void Sample() override;
		bool UpdateSampleIntervals(UINT samplesU, UINT samplesV);

		UINT m_samplesU;
		UINT m_samplesV;
	};
}
