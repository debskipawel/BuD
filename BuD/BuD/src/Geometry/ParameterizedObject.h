#pragma once

#include "DirectX11/DX11Device.h"
#include "Scene/RenderableSceneEntity.h"

#include <SimpleMath.h>
#include <functional>

using namespace DirectX::SimpleMath;

namespace BuD
{
	template<typename Dom>
	class ParameterizedObject
	{
	public:
		ParameterizedObject(std::function<Vector3(Dom)> objectFunction, Dom minDomain, Dom maxDomain)
			: m_min(minDomain), m_max(maxDomain), m_function(objectFunction)
		{
		}
		
		virtual void UpdateRenderableModel() = 0;

		inline std::shared_ptr<RenderableSceneEntity> GetModel() { return m_model; }
		inline Vector3 GetPoint(Dom parameter) { return m_function(parameter); }

	protected:
		virtual void Sample() = 0;

		std::shared_ptr<RenderableSceneEntity> m_model;

		Dom m_min;
		Dom m_max;
		Dom m_prevInterval;

		std::vector<Vector3> m_vertices;
		std::vector<unsigned short> m_indices;

		std::function<Vector3(Dom)> m_function;
	};
}
