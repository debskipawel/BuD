#pragma once

#include <SimpleMath.h>

namespace dx = DirectX;
namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class SceneEntity
	{
	public:
		virtual dxm::Matrix GetModelMatrix();

	protected:
		SceneEntity() = default;

		dxm::Vector3 m_position = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_rotation = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_scale = { 1.0f, 1.0f, 1.0f };
	};
}
