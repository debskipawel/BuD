#pragma once

enum class ObjectFlags
{
	NONE = 0x00000000,

	POINT = 0x00000001,
	POINT_BASED = 0x00000002,
	CURVE = 0x00000004 | POINT_BASED,
	PATCH = 0x00000008 | POINT_BASED,
	SURFACE = 0x00000010,
	TORUS = 0x00010000,
	
	C0 = 0x00100000,
	C2 = 0x00200000,
	INTERPOLATED = 0x00400000,
	
	BEZIER_CURVE_C0 = CURVE | C0,
	BEZIER_CURVE_C2 = CURVE | C2,
	INTERPOLATED_CURVE_C2 = INTERPOLATED | CURVE | C2,

	BEZIER_PATCH_C0 = PATCH | C0,
	BEZIER_PATCH_C2 = PATCH | C2,

	SURFACE_C0 = SURFACE | C0,
	SURFACE_C2 = SURFACE | C2
};

inline ObjectFlags operator|(ObjectFlags a, ObjectFlags b)
{
	return static_cast<ObjectFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline ObjectFlags operator&(ObjectFlags a, ObjectFlags b)
{
	return static_cast<ObjectFlags>(static_cast<int>(a) & static_cast<int>(b));
}
