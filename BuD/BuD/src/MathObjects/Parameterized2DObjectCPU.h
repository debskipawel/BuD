#pragma once

#include "ParameterizedObjectCPU.h"

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class Parameterized2DObjectCPU : public ParameterizedObjectCPU<Vector2>
	{
	public:
		Parameterized2DObjectCPU() = default;

		virtual void Meshify() override;

		virtual Vector2 MinDomain() override = 0;
		virtual Vector2 MaxDomain() override = 0;

		virtual void UpdateSamples(int samplesU, int samplesV)
		{
			m_samplesU = samplesU;
			m_samplesV = samplesV;
		}

		inline virtual int SamplesU() { return m_samplesU; }
		inline virtual int SamplesV() { return m_samplesV; }

		virtual bool ParameterizationWrapped() override = 0;

	protected:
		virtual Vector3 GetPoint(Vector2 parameter) = 0;

		int m_samplesU, m_samplesV;
	};
}
