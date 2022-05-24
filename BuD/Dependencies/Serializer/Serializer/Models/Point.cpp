#include <pch.h>

#include "Point.h"

#include <nlohmann/json.hpp>

namespace MG1
{
	std::map<uint32_t, Point*> Point::s_pointMap = {};
	
	Point::Point()
		: SceneObject(), position()
	{
		s_pointMap.emplace(std::make_pair(m_id, this));
	}

	bool Point::SetId(uint32_t id)
	{
		auto prevId = m_id;

		if (!SceneObject::SetId(id))
		{
			return false;
		}

		s_pointMap.erase(prevId);
		s_pointMap.emplace(std::make_pair(id, this));

		return true;
	}

	void to_json(nlohmann::json& j, const Point& p)
	{
		j = {
			{ "id", p.m_id },
			{ "name", p.name.c_str() },
			{ "position", p.position }
		};
	}

	void from_json(const nlohmann::json& j, Point& p)
	{
		Float3 position = j["position"];
		
		auto id = j["id"].get<uint32_t>();

		// TODO: check if the point was not a duplicate - SetId will return false
		p.SetId(id);
		p.position = position;
	}
}
