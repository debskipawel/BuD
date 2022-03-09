#pragma once

#include "AbstractCamera.h"

#include <gmtl/gmtl.h>

namespace BuD
{
	class OrthographicCamera : public AbstractCamera
	{
	public:
		OrthographicCamera(const Vector3D& position, const Vector3D& front, const Vector3D& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

	protected:
		virtual void UpdateProjectionMatrix() override;
	};
}
