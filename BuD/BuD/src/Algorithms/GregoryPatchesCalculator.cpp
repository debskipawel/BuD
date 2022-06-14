#include "GregoryPatchesCalculator.h"

namespace BuD
{
	std::vector<std::vector<Vector3>> GregoryPatchCalculator::CalculateControlPoints(const std::vector<Point*>& firstLine, const std::vector<Point*>& secondLine)
	{
		auto [firstLineSplit, secondLineSplit] = CalculateSplitSides(firstLine, secondLine);
		auto patchesCount = firstLine.size() / 3;

		return std::vector<std::vector<Vector3>>();
	}

	std::pair<std::vector<std::vector<Vector3>>, std::vector<std::vector<Vector3>>> GregoryPatchCalculator::CalculateSplitSides(const std::vector<Point*>& firstLine, const std::vector<Point*>& secondLine)
	{
		auto patchesCount = firstLine.size() / 3;

		std::vector<std::vector<Vector3>> splitFirstSides, splitSecondSides;

		for (int i = 0; i < patchesCount; i++)
		{
			std::vector<Vector3> firstSide =
			{
				firstLine[i * 3]->Position(),
				firstLine[i * 3 + 1]->Position(),
				firstLine[i * 3 + 2]->Position(),
				firstLine[(i * 3 + 3) % (firstLine.size())]->Position()
			};

			std::vector<Vector3> secondSide =
			{
				secondLine[i * 4]->Position(),
				secondLine[i * 4 + 1]->Position(),
				secondLine[i * 4 + 2]->Position(),
				secondLine[i * 4 + 3]->Position()
			};

			splitFirstSides.push_back(SplitSide(firstSide));
			splitSecondSides.push_back(SplitSide(secondSide));
		}

		return std::make_pair(splitFirstSides, splitSecondSides);
	}
	
	std::vector<Vector3> GregoryPatchCalculator::SplitSide(const std::vector<Vector3>& side)
	{
		std::vector<Vector3> sideSplit =
		{
			side[0],
			(side[0] + side[1]) / 2.0f,
			(side[0] + 2 * side[1] + side[2]) / 4.0f,
			(side[0] + 3 * side[1] + 3 * side[2] + side[3]) / 8.0f,
			(side[1] + 2 * side[2] + side[3]) / 4.0f,
			(side[2] + side[3]) / 2.0f,
			side[3]
		};

		return sideSplit;
	}
}