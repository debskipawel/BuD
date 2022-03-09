#include "CameraFactory.h"

#include "OrthographicCamera.h"

std::shared_ptr<BuD::AbstractCamera> BuD::CameraFactory::MakeOrthographic(const dx::XMFLOAT3& position, const dx::XMFLOAT3& front, const dx::XMFLOAT3& worldUp, float ratio)
{
	return std::make_shared<OrthographicCamera>(position, front, worldUp, ratio);
}
