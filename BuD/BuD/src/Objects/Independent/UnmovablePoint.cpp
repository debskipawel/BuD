#include "UnmovablePoint.h"

#include <Objects/Scene.h>

namespace BuD
{
	UnmovablePoint::UnmovablePoint(Scene& scene, const DX11Device& device, const Vector3& position)
		: Point(scene, device, position)
	{
	}
}