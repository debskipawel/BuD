#pragma once

#include "AbstractCamera.h"

namespace BuD
{
	class StereoscopicCamera : public AbstractCamera
	{
	public:
		StereoscopicCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

		virtual const dxm::Matrix& GetLeftEyeProjection() const override { return m_leftEyeProjection; }
		virtual const dxm::Matrix& GetRightEyeProjection() const override { return m_rightEyeProjection; }

		virtual void DrawGui() override;

	protected:
		virtual void UpdateProjectionMatrix() override;

		dxm::Matrix m_leftEyeProjection;
		dxm::Matrix m_rightEyeProjection;

		float m_eyeDistance = 6.0f;
		float m_focus = 20.0f;
	};
}
