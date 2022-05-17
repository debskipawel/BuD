#pragma once

#include "Point.h"

namespace BuD
{
	class UnmovablePoint : public Point
	{
	public:
		UnmovablePoint(Scene& scene, const DX11Device& device, const Vector3& position);

		virtual void MoveTo(const Vector3& position, bool propagateUpdate = false) override {}
		virtual void MoveBy(const Vector3& difference, bool propagateUpdate = false) override {}
	};
}
