#pragma once

#include <Objects/Abstract/SceneObject.h>

#include <memory>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class Ray
	{
		Vector3 origin;
		Vector3 direction;
	};
}
