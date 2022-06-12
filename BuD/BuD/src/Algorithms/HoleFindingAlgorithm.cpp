#include "HoleFindingAlgorithm.h"

#include <algorithm>
#include <iterator>

#include <Objects/PointBased/Surfaces/BezierPatchC0.h>

namespace BuD
{
	std::vector<Point*> HoleFindingAlgorithm::FindHole(std::vector<BezierPatchC0*> patches)
	{
		if (patches.size() < 3)
		{
			return {};
		}

		std::vector<Point*> cycle;

		auto [currentPoint, startPatch] = FindStartingPoint(patches);

		if (currentPoint == nullptr)
		{
			return {};
		}

		auto currentPatch = startPatch;

		do
		{
			auto patchesContaining = GetAllPatchesContaining(patches, currentPoint);
			auto newPointFound = false;

			// if no patch containing the point can give us a new point on cycle, {} needs to be returned
			for (auto& patch : patchesContaining)
			{
				auto& cp = patch->m_controlPoints;

				if (patch == currentPatch)
				{
					continue;
				}

				auto nextPoint = FindNextPointOnCycle(patches, currentPoint, currentPatch, patch);

				if (nextPoint == nullptr)
				{
					continue;
				}

				newPointFound = true;

				// add points between currentPoint and nextPoint to the cycle
				auto nextIndex = static_cast<int>(std::find(cp.begin(), cp.end(), nextPoint) - cp.begin());
				auto prevIndex = static_cast<int>(std::find(cp.begin(), cp.end(), currentPoint) - cp.begin());
				auto deltaIndex = (nextIndex - prevIndex) / 3;

				for (int i = 0; i < 3; i++)
				{
					auto newPointOnCycle = cp[prevIndex + i * deltaIndex];

					if (std::find(cycle.begin(), cycle.end(), newPointOnCycle) != cycle.end())
					{
						return {};
					}

					cycle.push_back(newPointOnCycle);
				}

				currentPatch = patch;
				currentPoint = nextPoint;

				break;
			}

			if (!newPointFound)
			{
				return {};
			}
		} while (currentPatch != startPatch);

		return cycle;
	}
	
	std::vector<BezierPatchC0*> HoleFindingAlgorithm::GetAllPatchesContaining(std::vector<BezierPatchC0*> patches, Point* point)
	{
		std::vector<BezierPatchC0*> patchesContaining;

		for (auto& patch : patches)
		{
			auto& cp = patch->m_controlPoints;

			if (std::find(cp.begin(), cp.end(), point) != cp.end())
			{
				patchesContaining.push_back(patch);
			}
		}

		return patchesContaining;
	}

	std::pair<Point*, BezierPatchC0*>  HoleFindingAlgorithm::FindStartingPoint(std::vector<BezierPatchC0*> patches)
	{
		const std::vector<int> potentialNeighbours = { 0, 3, 12, 15 };

		int maxIntersectionSize = 0;

		for (int i = 0; i < patches.size(); i++)
		{
			auto iControlPoints = patches[i]->m_controlPoints;
			std::sort(iControlPoints.begin(), iControlPoints.end());

			for (int j = i + 1; j < patches.size(); j++)
			{
				auto jControlPoints = patches[j]->m_controlPoints;
				std::sort(jControlPoints.begin(), jControlPoints.end());

				std::vector<Point*> intersection;
				std::set_intersection(iControlPoints.begin(), iControlPoints.end(), jControlPoints.begin(), jControlPoints.end(), std::back_inserter(intersection));

				if (intersection.size() > maxIntersectionSize)
				{
					maxIntersectionSize = intersection.size();
				}

				if (intersection.size() != 1)
				{
					continue;
				}

				auto point = intersection[0];

				auto indexInI = static_cast<int>(std::find(iControlPoints.begin(), iControlPoints.end(), point) - iControlPoints.begin());
				auto indexInJ = static_cast<int>(std::find(jControlPoints.begin(), jControlPoints.end(), point) - jControlPoints.begin());

				if (std::find(potentialNeighbours.begin(), potentialNeighbours.end(), indexInI) == potentialNeighbours.end()
					&& std::find(potentialNeighbours.begin(), potentialNeighbours.end(), indexInJ) == potentialNeighbours.end())
				{
					continue;
				}

				return { point, patches[i] };
			}
		}

		if (maxIntersectionSize == 0)
		{
			return { nullptr, nullptr };
		}

		auto& startingPatch = patches[0];
		auto& cp = startingPatch->m_controlPoints;

		for (auto neighbour : potentialNeighbours)
		{
			auto& point = cp[neighbour];

			for (auto& patch : patches)
			{
				auto& patchCP = patch->m_controlPoints;

				if (patch == startingPatch || std::find(patchCP.begin(), patchCP.end(), point) == patchCP.end())
				{
					continue;
				}

				return { point, patch };
			}
		}

		return { nullptr, nullptr };
	}

	Point* HoleFindingAlgorithm::FindNextPointOnCycle(std::vector<BezierPatchC0*> patches, Point* currPoint, BezierPatchC0* prev, BezierPatchC0* next)
	{
		auto& prevCP = prev->m_controlPoints;
		auto& currCP = next->m_controlPoints;

		const std::set<int> allowedIndices = { 0, 3, 12, 15 };
		const std::unordered_map<int, std::vector<int>> potentialNeighboursMap =
		{
			{ 0, { 3, 12 }},
			{ 3, { 0, 15 }},
			{ 12, { 0, 15 }},
			{ 15, { 3, 12 }},
		};

		auto index = static_cast<int>(std::find(currCP.begin(), currCP.end(), currPoint) - currCP.begin());

		if (allowedIndices.find(index) == allowedIndices.end())
		{
			return nullptr;
		}

		auto& potentialNeighbours = potentialNeighboursMap.at(index);

		for (auto neighbour : potentialNeighbours)
		{
			auto& point = currCP[neighbour];

			auto containingPatches = GetAllPatchesContaining(patches, point);

			if (containingPatches.size() == 1 || std::find(prevCP.begin(), prevCP.end(), point) != prevCP.end())
			{
				continue;
			}

			return point;
		}

		return nullptr;
	}
}
