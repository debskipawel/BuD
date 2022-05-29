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

		auto wrapped = nlohmann::json::object();
		wrapped["u"] = p.uWrapped;
		wrapped["v"] = p.vWrapped;

		j = {
			{ "name", p.name },
			{ "id", p.m_id },
			{ "size", p.size },
			{ "parameterWrapped", wrapped },
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

		p.size = j["size"];

		auto& wrapped = j["parameterWrapped"];
		p.uWrapped = wrapped["u"].get<bool>();
		p.vWrapped = wrapped["v"].get<bool>();

		auto id = j["id"].get<uint32_t>();
		p.SetId(id);
	}
}
