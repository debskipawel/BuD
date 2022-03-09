#pragma once

#include "AbstractCamera.h"

#include <gmtl/Vec.h>

#include <memory>

namespace BuD
{
	class CameraFactory
	{
	public:
		static std::shared_ptr<AbstractCamera> MakeOrthographic(const Vector3D& position, const Vector3D& front, const Vector3D& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

	protected:
		CameraFactory() = default;
		virtual ~CameraFactory() = default;
	};
}