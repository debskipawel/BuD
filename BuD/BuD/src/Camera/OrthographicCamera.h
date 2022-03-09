#pragma once

#include "AbstractCamera.h"

namespace BuD
{
	class OrthographicCamera : public AbstractCamera
	{
	public:
		OrthographicCamera(const dx::XMFLOAT3& position, const dx::XMFLOAT3& front, const dx::XMFLOAT3& worldUp = { 0.0f, 1.0f, 0.0f }, float ratio = 1.0f);

	protected:
		virtual void UpdateProjectionMatrix() override;
	};
}
