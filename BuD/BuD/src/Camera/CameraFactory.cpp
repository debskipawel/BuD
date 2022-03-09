#include "CameraFactory.h"

#include "OrthographicCamera.h"

std::shared_ptr<BuD::AbstractCamera> BuD::CameraFactory::MakeOrthographic(const Vector3D& position, const Vector3D& front, const Vector3D& worldUp, float ratio)
{
	return std::make_shared<OrthographicCamera>(position, front, worldUp, ratio);
}
