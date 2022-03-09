#pragma once

#include "SimpleMath.h"

namespace dx = DirectX;
namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class AbstractCamera
	{
	public:
		void LookAt(const dxm::Vector3& target);

		dxm::Matrix GetViewMatrix();
		dxm::Matrix GetProjectionMatrix();

		void Move(const dxm::Vector3& difference);
		void MoveTo(const dxm::Vector3& position);

		inline void UpdateAspectRatio(float ratio)
		{
			m_projRight = ratio;
			m_projLeft = -ratio;

			UpdateProjectionMatrix();
		}

	protected:
		AbstractCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;

		dxm::Matrix m_viewMatrix;
		dxm::Matrix m_projectionMatrix;

		dxm::Vector3 m_position;
		dxm::Vector3 m_front;
		dxm::Vector3 m_right;
		dxm::Vector3 m_up;
		dxm::Vector3 m_worldUp;

		const float m_projFar = -1.0f;
		const float m_projNear = 0.0f;
		float m_projTop = 1.0f;
		float m_projBottom = -1.0f;
		float m_projLeft = -1.0f;
		float m_projRight = 1.0f;

		float m_pitch;
		float m_yaw;
	};
}
