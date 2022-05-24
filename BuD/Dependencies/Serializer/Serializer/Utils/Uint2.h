#pragma once

#include <Serializable.h>

namespace MG1
{
	class Uint2
	{
	public:
		uint32_t x, y;

		friend void to_json(nlohmann::json& j, const Uint2& p);
		friend void from_json(const nlohmann::json& j, Uint2& p);
	};
}
