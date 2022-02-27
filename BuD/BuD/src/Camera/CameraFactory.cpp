#include "CameraFactory.h"

#include "OrthographicCamera.h"

std::shared_ptr<BuD::AbstractCamera> BuD::CameraFactory::MakeOrthographic(gmtl::Vec3f position, gmtl::Vec3f front, gmtl::Vec3f worldUp, float ratio)
{
	return std::make_shared<OrthographicCamera>(position, front, worldUp, ratio);
}
