#include "pch.h"
#include "Uint2.h"

#include <nlohmann/json.hpp>

namespace MG1
{
	void to_json(nlohmann::json& j, const Uint2& p)
	{
		j = {
			{ "x", p.x },
			{ "y", p.y }
		};
	}
	
	void from_json(const nlohmann::json& j, Uint2& p)
	{
		p.x = j["x"].get<uint32_t>();
		p.y = j["y"].get<uint32_t>();
	}
}
