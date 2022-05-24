#pragma once

#include "Bezier.h"

namespace MG1
{
	class BezierC0 : public Bezier
	{
	public:
		friend void to_json(nlohmann::json& j, const BezierC0& p);
		friend void from_json(const nlohmann::json& j, BezierC0& p);
	};
}
