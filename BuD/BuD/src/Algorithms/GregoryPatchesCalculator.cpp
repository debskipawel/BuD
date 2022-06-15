#include "GregoryPatchesCalculator.h"

#include <algorithm>
#include <array>
#include <numeric>

namespace BuD
{
	std::vector<std::vector<Vector3>> GregoryPatchCalculator::CalculateControlPoints(const std::vector<Point*>& firstLine, const std::vector<Point*>& secondLine)
	{
		auto patchesCount = firstLine.size() / 3;
		std::vector<std::vector<Vector3>> gregoryPatches;

		// 1. Obliczyc punkty p^3_i dzielac wejsciowe platki
		auto [firstLineSplit, secondLineSplit] = CalculateSplitSides(firstLine, secondLine);

		for (int i = 0; i < patchesCount; i++)
		{
			gregoryPatches.push_back(std::vector<Vector3>(20));

			std::vector<Vector3> prevFirstLine = firstLineSplit[i];
			std::vector<Vector3> nextFirstLine = firstLineSplit[(i + 1) % patchesCount];

			int sideIndicesV[] = { 3, 9, 15, 19 };
			for (int j = 0; j < 4; j++)
			{
				gregoryPatches[i][j] = prevFirstLine[3 + j];
				gregoryPatches[i][sideIndicesV[j]] = nextFirstLine[j];
			}
		}

		// 2. Obliczyc punkty p^2_i na podstawie warunkow ciaglosci C1
		for (int i = 0; i < patchesCount; i++)
		{
			std::vector<Vector3> prevSecondLine = secondLineSplit[i];
			std::vector<Vector3> nextSecondLine = secondLineSplit[(i + 1) % patchesCount];

			int sideIndicesU[] = { 0, 1, 1, 2 };
			int sideIndicesV[] = { 8, 13, 14, 18 };
			int sideIndicesVPrev[] = { 9, 15, 15, 19 };
			
			for (int j = 0; j < 4; j++)
			{
				gregoryPatches[i][4 + j] = 1.33f * gregoryPatches[i][sideIndicesU[j]] - 0.33f * prevSecondLine[3 + sideIndicesU[j]];
			}

			for (int j = 0; j < 4; j++)
			{
				gregoryPatches[i][sideIndicesV[j]] = 1.33f * gregoryPatches[i][sideIndicesVPrev[j]] - 0.33f * nextSecondLine[sideIndicesU[j] + 1];
			}
		}

		// 3. Obliczyc punkty pomocnicze dla kazdego platka
		std::vector<Vector3> qPoints(patchesCount);

		for (int i = 0; i < patchesCount; i++)
		{
			qPoints[i] = (3.0f * gregoryPatches[i][4] - gregoryPatches[i][0]) / 2.0f;
		}

		auto centerOfMass = std::accumulate(qPoints.begin(), qPoints.end(), Vector3(0.0f)) / patchesCount;

		for (int i = 0; i < patchesCount; i++)
		{
			gregoryPatches[i][16] = centerOfMass;
		}

		// 4. Na podstawie punktow Q obliczyc punkty p^1_i
		for (int i = 0; i < patchesCount; i++)
		{
			gregoryPatches[i][10] = (2.0f * qPoints[i] + centerOfMass) / 3.0f;
			gregoryPatches[i][17] = (2.0f * qPoints[(i + 1) % patchesCount] + centerOfMass) / 3.0f;
		}

		// 5. Obliczyc dwa tycki - indeksy 11 i 12
		for (int i = 0; i < patchesCount; i++)
		{
			gregoryPatches[i][11] = gregoryPatches[i][12] = (gregoryPatches[i][16] + gregoryPatches[i][10] + gregoryPatches[i][17]) / 3.0f;
		}

		return gregoryPatches;
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