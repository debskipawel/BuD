#include "pch.h"
#include "BezierPatchC2.h"

#include <nlohmann/json.hpp>
#include <algorithm>

namespace MG1
{
	void to_json(nlohmann::json& j, const BezierPatchC2& p)
	{
		auto points = nlohmann::json::array();

		std::transform(
			p.controlPoints.begin(), p.controlPoints.end(),
			std::back_inserter(points),
			[](const PointRef& point) { return nlohmann::json{ { "id", point.GetId() } }; }
		);

		j = {
			{ "objectType", "bezierPatchC2" },
			{ "id", p.m_id },
			{ "name", p.name },
			{ "controlPoints", points },
			{ "samples", p.samples }
		};
	}
	
	void from_json(const nlohmann::json& j, BezierPatchC2& p)
	{
		if (j.contains("name"))
		{
			p.name = j["name"];
		}

		p.controlPoints.clear();

		for (auto& pointRef : j["controlPoints"])
		{
			auto id = pointRef["id"].get<uint32_t>();
			auto ref = PointRef(id);
			p.controlPoints.push_back(ref);
		}

		p.samples = j["samples"];
		p.SetId(j["id"].get<uint32_t>());
	}
}
