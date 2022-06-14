#pragma once

#include <vector>
#include <SimpleMath.h>

#include <Objects/Independent/Point.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class GregoryPatchCalculator
	{
	public:
		static std::vector<std::vector<Vector3>> CalculateControlPoints(
			const std::vector<Point*>& firstLine, 
			const std::vector<Point*>& secondLine
		);

	private:
		static std::pair<std::vector<std::vector<Vector3>>, std::vector<std::vector<Vector3>>> CalculateSplitSides(
			const std::vector<Point*>& firstLine,
			const std::vector<Point*>& secondLine
		);
		static std::vector<Vector3> SplitSide(const std::vector<Vector3>& side);
	};
}
