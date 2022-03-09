#pragma once

#include <DirectXMath.h>

namespace dx = DirectX;

namespace BuD
{
	class AbstractCamera
	{
	public:
		void LookAt(const dx::XMFLOAT3& target);

		dx::XMMATRIX GetViewMatrix();
		dx::XMMATRIX GetProjectionMatrix();

		void Move(const dx::XMFLOAT3& difference);
		void MoveTo(const dx::XMFLOAT3& position);

		inline void UpdateAspectRatio(float ratio)
		{
			m_projRight = ratio;
			m_projLeft = -ratio;

			UpdateProjectionMatrix();
		}

	protected:
		AbstractCamera(const dx::XMFLOAT3& position, const dx::XMFLOAT3& front, const dx::XMFLOAT3& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;

		dx::XMMATRIX m_viewMatrix;
		dx::XMMATRIX m_projectionMatrix;

		dx::XMFLOAT3 m_position;
		dx::XMFLOAT3 m_front;
		dx::XMFLOAT3 m_right;
		dx::XMFLOAT3 m_up;
		dx::XMFLOAT3 m_worldUp;

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
