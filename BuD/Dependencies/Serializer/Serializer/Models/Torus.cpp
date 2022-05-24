#include "pch.h"
#include "Torus.h"

#include <nlohmann/json.hpp>

namespace MG1
{
	void to_json(nlohmann::json& j, const Torus& p)
	{
		j = {
			{ "id", p.m_id },
			{ "name", p.name },
			{ "smallRadius", p.smallRadius },
			{ "largeRadius", p.largeRadius },
			{ "position", p.position },
			{ "rotation", p.rotation },
			{ "scale", p.scale },
			{ "samples", p.samples }
		};
	}
	
	void from_json(const nlohmann::json& j, Torus& p)
	{
		if (j.contains("name"))
		{
			p.name = j["name"];
		}

		p.position = j["position"];
		p.rotation = j["rotation"];
		p.scale = j["scale"];

		p.samples = j["samples"];

		p.smallRadius = j["smallRadius"].get<float>();
		p.largeRadius = j["largeRadius"].get<float>();

		p.SetId(j["id"].get<uint32_t>());
	}
}
