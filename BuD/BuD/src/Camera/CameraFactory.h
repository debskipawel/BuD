#pragma once

#include "AbstractCamera.h"

#include <memory>

namespace BuD
{
	class CameraFactory
	{
	public:
		static std::shared_ptr<AbstractCamera> MakeOrthographic(
			const dxm::Vector3& position, 
			const dxm::Vector3& front, 
			const dxm::Vector3& worldUp = { 0.0f, 1.0f, 0.0f }, 
			float ratio = 1.0f
		);

		static std::shared_ptr<AbstractCamera> MakePerspective(
			const dxm::Vector3& position,
			const dxm::Vector3& front,
			const dxm::Vector3& worldUp = { 0.0f, 1.0f, 0.0f },
			float ratio = 1.0f
		);

		static std::shared_ptr<AbstractCamera> MakeStereoscopic(
			const dxm::Vector3& position,
			const dxm::Vector3& front,
			const dxm::Vector3& worldUp = { 0.0f, 1.0f, 0.0f },
			float ratio = 1.0f
		);

	protected:
		CameraFactory() = default;
		virtual ~CameraFactory() = default;
	};
}