#pragma once

#include "AbstractCamera.h"

#include <memory>

namespace BuD
{
	class CameraFactory
	{
	public:
		static std::shared_ptr<AbstractCamera> MakeOrthographic(const dx::XMFLOAT3& position, const dx::XMFLOAT3& front, const dx::XMFLOAT3& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

	protected:
		CameraFactory() = default;
		virtual ~CameraFactory() = default;
	};
}