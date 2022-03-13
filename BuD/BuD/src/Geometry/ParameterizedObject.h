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

		virtual void SampleBy(Dom interval) = 0;
		virtual void UpdateModel(const DX11Device& device) = 0;

		inline std::shared_ptr<RenderableSceneEntity> GetModel() { return m_model; }
		inline Vector3 GetPoint(Dom parameter) { return m_function(parameter); }

	protected:
		std::shared_ptr<RenderableSceneEntity> m_model;

		Dom m_min;
		Dom m_max;

		std::function<Vector3(Dom)> m_function;
	};
}
