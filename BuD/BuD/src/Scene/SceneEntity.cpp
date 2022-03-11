#include "SceneEntity.h"

namespace BuD
{
	dxm::Matrix SceneEntity::GetModelMatrix()
	{
		auto rot = dxm::Matrix::CreateRotationX(dx::XMConvertToRadians(m_rotation.x))
			* dxm::Matrix::CreateRotationY(dx::XMConvertToRadians(m_rotation.y))
			* dxm::Matrix::CreateRotationZ(dx::XMConvertToRadians(m_rotation.z));

		auto trans = dxm::Matrix::CreateTranslation(m_position);

		auto scale = dxm::Matrix::CreateScale(m_scale);

		return trans * rot * scale;
	}
}
