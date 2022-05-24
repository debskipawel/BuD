#include "pch.h"
#include "BezierSurfaceC0.h"

#include <nlohmann/json.hpp>

namespace MG1
{
	void to_json(nlohmann::json& j, const BezierSurfaceC0& p)
	{
		auto patches = nlohmann::json::array();

		for (auto& patch : p.patches)
		{
			patches.push_back(patch);
		}

		j = {
			{ "name", p.name },
			{ "id", p.m_id },
			{ "objectType", "bezierSurfaceC0" },
			{ "patches", patches }
		};
	}

	void from_json(const nlohmann::json& j, BezierSurfaceC0& p)
	{
		for (auto& patch : j["patches"])
		{
			p.patches.push_back(patch);
		}

		if (j.contains("name"))
		{
			p.name = j["name"];
		}

		auto id = j["id"].get<uint32_t>();
		p.SetId(id);
	}
}
