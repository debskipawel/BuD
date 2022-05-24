#pragma once

#include "Bezier.h"

namespace MG1
{
	class InterpolatedC2 : public Bezier
	{
	public:
		friend void to_json(nlohmann::json& j, const InterpolatedC2& p);
		friend void from_json(const nlohmann::json& j, InterpolatedC2& p);
	};
}
