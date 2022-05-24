#pragma once

#include "BezierPatch.h"

namespace MG1
{
	class BezierPatchC0 : public BezierPatch
	{
	public:
		friend void to_json(nlohmann::json& j, const BezierPatchC0& p);
		friend void from_json(const nlohmann::json& j, BezierPatchC0& p);
	};
}
