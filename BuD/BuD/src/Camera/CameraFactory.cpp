#include "CameraFactory.h"

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "StereoscopicCamera.h"

namespace BuD
{
	std::shared_ptr<AbstractCamera> CameraFactory::MakeOrthographic(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
	{
		return std::make_shared<OrthographicCamera>(position, front, worldUp, ratio);
	}

	std::shared_ptr<AbstractCamera> CameraFactory::MakePerspective(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
	{
		return std::make_shared<PerspectiveCamera>(position, front, worldUp, ratio);
	}

	std::shared_ptr<AbstractCamera> CameraFactory::MakeStereoscopic(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
	{
		return std::make_shared<StereoscopicCamera>(position, front, worldUp, ratio);
	}
}
