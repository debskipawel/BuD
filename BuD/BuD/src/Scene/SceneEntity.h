#pragma once

#include <SimpleMath.h>

#include <Editors/IEditable.h>

namespace dx = DirectX;
namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class SceneEntity : public IEditable
	{
	public:
		virtual void DrawGui() override;

		virtual dxm::Matrix GetModelMatrix();

		dxm::Vector3 m_position = { 0.0f, 0.0f, -2.0f };
		dxm::Vector3 m_rotation = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_scale = { 1.0f, 1.0f, 1.0f };

	protected:
		SceneEntity() = default;
	};
}
