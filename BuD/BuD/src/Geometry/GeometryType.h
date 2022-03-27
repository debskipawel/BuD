#pragma once

namespace BuD
{
	enum class GeometryType : unsigned int
	{
		POINT =		0x00000001,
		TORUS =		0x00000002,
		BEZIER_C0 = 0x00000004,
		MIXED =		0x10000000,
		EMPTY =		0x00000000,
	};
}
