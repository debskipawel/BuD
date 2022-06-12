#pragma once

#include <vector>

#include <Objects/Independent/Point.h>

namespace BuD
{
	class BezierPatchC0;

	class HoleFindingAlgorithm
	{
	public:
		HoleFindingAlgorithm() = delete;
		HoleFindingAlgorithm(const HoleFindingAlgorithm& other) = delete;
		HoleFindingAlgorithm(HoleFindingAlgorithm&& other) = delete;

		static std::vector<Point*> FindHole(std::vector<BezierPatchC0*> patches); 

	protected:
		static std::vector<BezierPatchC0*> GetAllPatchesContaining(std::vector<BezierPatchC0*> patches, Point* point);
		static std::pair<Point*, BezierPatchC0*> FindStartingPoint(std::vector<BezierPatchC0*> patches);
		static Point* FindNextPointOnCycle(std::vector<BezierPatchC0*> patches, Point* currPoint, BezierPatchC0* prev, BezierPatchC0* next);
	};
}
