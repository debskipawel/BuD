#pragma once

#include "ParameterizedObject.h"

namespace BuD
{
	template <typename Dom>
	class ParameterizedObjectCPU : ParameterizedObject<Dom>
	{
	public:
		virtual void Meshify() = 0;

	protected:
		virtual Vector3 GetPoint(Dom parameter) = 0;

		std::vector<Vector3> m_vertices;
		std::vector<unsigned short> m_indices;
	};
}
