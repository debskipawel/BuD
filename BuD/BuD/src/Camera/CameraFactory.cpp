#include "CameraFactory.h"

#include "OrthographicCamera.h"

std::shared_ptr<BuD::AbstractCamera> BuD::CameraFactory::MakeOrthographic(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& worldUp, float ratio)
{
	return std::make_shared<OrthographicCamera>(position, front, worldUp, ratio);
}
