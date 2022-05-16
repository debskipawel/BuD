#include "UnmovablePoint.h"

namespace BuD
{
	UnmovablePoint::UnmovablePoint(const DX11Device& device, const Vector3& position)
		: Point(device, position)
	{
	}
}