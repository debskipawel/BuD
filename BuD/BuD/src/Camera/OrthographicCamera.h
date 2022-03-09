#pragma once

#include "AbstractCamera.h"

namespace BuD
{
	class OrthographicCamera : public AbstractCamera
	{
	public:
		OrthographicCamera(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

	protected:
		virtual void UpdateProjectionMatrix() override;
	};
}
