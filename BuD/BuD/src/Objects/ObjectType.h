#pragma once

enum class ObjectType
{
	NONE = 0x00000000,

	POINT = 0x00000001,
	POINT_BASED = 0x00000002,
	CURVE = 0x00000004 | POINT_BASED,
	TORUS = 0x00010000,
	
	C0 = 0x00100000,
	C2 = 0x00200000,
	INTERPOLATED = 0x00400000,
	
	BEZIER_CURVE_C0 = CURVE | C0,
	BEZIER_CURVE_C2 = CURVE | C2,
	INTERPOLATED_CURVE_C2 = INTERPOLATED | CURVE | C2
};

inline ObjectType operator|(ObjectType a, ObjectType b)
{
	return static_cast<ObjectType>(static_cast<int>(a) | static_cast<int>(b));
}

inline ObjectType operator&(ObjectType a, ObjectType b)
{
	return static_cast<ObjectType>(static_cast<int>(a) & static_cast<int>(b));
}
