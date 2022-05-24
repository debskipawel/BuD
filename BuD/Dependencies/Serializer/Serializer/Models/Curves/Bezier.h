#pragma once

#include <vector>

#include <Serializable.h>

#include <Models/SceneObject.h>
#include <Models/PointRef.h>

namespace MG1
{
	class Bezier : public SceneObject
	{
	public:
		explicit Bezier() = default;

		std::vector<PointRef> controlPoints;

		friend void to_json(nlohmann::json& j, const Bezier& p);
		friend void from_json(const nlohmann::json& j, Bezier& p);
	};
}
