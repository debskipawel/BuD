#pragma once

#include <SimpleMath.h>
#include <functional>

using namespace DirectX::SimpleMath;

namespace BuD
{
	template<typename Dom>
	class ParameterizedObject
	{
	public:
		ParameterizedObject() = default;

		virtual Vector3 GetPoint(Dom parameter) = 0;

		virtual Dom MinDomain() = 0;
		virtual Dom MaxDomain() = 0;

		virtual bool ParameterizationWrapped() = 0;
	};
}
