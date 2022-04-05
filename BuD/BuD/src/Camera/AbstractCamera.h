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

		inline dxm::Vector3 Position() const { return m_position; }
		inline dxm::Vector3 Front() const { return m_front; }
		inline dxm::Vector3 Up() const { return m_up; }
		inline dxm::Vector3 Right() const { return m_right; }

		const dxm::Matrix& GetViewMatrix() const;
		const dxm::Matrix& GetProjectionMatrix() const;

		void Move(const dxm::Vector3& difference);
		void MoveTo(const dxm::Vector3& position);

		void ProcessMouseMovement(int xOffset, int yOffset);

		dxm::Vector3 MoveWorldPointToPixels(dxm::Vector3 point, int x, int y);

		inline void UpdateViewport(uint32_t width, uint32_t height)
		{
			m_width = width;
			m_height = height;

			float ratio = static_cast<float>(width) / height;

			m_projLeft = -ratio / 2;
			m_projRight = ratio / 2;

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

		uint32_t m_width, m_height;

		const float m_projFar = 100.0f;
		const float m_projNear = 0.01f;
		float m_projTop = 1.0f;
		float m_projBottom = 0.0f;
		float m_projLeft = -1.0f;
		float m_projRight = 1.0f;

		float m_pitch;
		float m_yaw;
	};
}
