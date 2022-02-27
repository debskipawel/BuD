#pragma once

#include "../Geometry/Vector3D.h"
#include "../Geometry/Matrix3D.h"

namespace BuD
{
	class AbstractCamera
	{
	public:
		void LookAt(const Vector3D& target);

		Matrix3D GetViewMatrix();
		Matrix3D GetProjectionMatrix();

		void Move(const Vector3D& difference);
		void MoveTo(const Vector3D& position);

		inline void UpdateAspectRatio(float ratio)
		{
			m_projRight = ratio;
			m_projLeft = -ratio;

			UpdateProjectionMatrix();
		}

	protected:
		AbstractCamera(const Vector3D& position, const Vector3D& front, const Vector3D& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;

		Matrix3D m_viewMatrix;
		Matrix3D m_projectionMatrix;

		Vector3D m_position;
		Vector3D m_front;
		Vector3D m_right;
		Vector3D m_up;
		Vector3D m_worldUp;

		const float m_projFar = -1.0f;
		const float m_projNear = 0.0f;
		const float m_projTop = 1.0f;
		const float m_projBottom = -1.0f;
		float m_projLeft = -1.0f;
		float m_projRight = 1.0f;
	};
}
