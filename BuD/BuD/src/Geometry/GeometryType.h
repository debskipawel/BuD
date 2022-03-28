#pragma once

namespace BuD
{
	enum class GeometryType : int
	{
		POINT =		0x00000001,
		TORUS =		0x00000002,
		BEZIER =	0x00000004,
		BEZIER_C0 = 0x00000008 | BEZIER,
		BEZIER_C2 = 0x00000010 | BEZIER,
		MIXED =		0x10000000,
		EMPTY =		0x00000000,
	};
}
