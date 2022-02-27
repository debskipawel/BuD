#pragma once

#include "AbstractCamera.h"

#include <gmtl/gmtl.h>

namespace BuD
{
	class OrthographicCamera : public AbstractCamera
	{
	public:
		OrthographicCamera(gmtl::Vec3f position, gmtl::Vec3f front, gmtl::Vec3f worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

	protected:
		virtual void UpdateProjectionMatrix() override;
	};
}
