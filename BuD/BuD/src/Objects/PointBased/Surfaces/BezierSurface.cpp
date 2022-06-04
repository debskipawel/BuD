#include "BezierSurface.h"

#include <Objects/Scene.h>

#include <unordered_set>

namespace BuD
{
	BezierSurface::BezierSurface(Scene& scene)
		: SceneObject(scene)
	{
	}

	BezierSurface::BezierSurface(Scene& scene, const std::vector<BezierPatch*>& patches)
		: SceneObject(scene), m_patches(patches)
	{
		for (auto& patch : m_patches)
		{
			patch->m_owner = this;
		}
	}

	void BezierSurface::MoveTo(const Vector3& position, bool propagateUpdate)
	{
		auto diff = position - m_patches[0]->m_controlPoints[0]->Position();

		MoveBy(diff);
	}

	void BezierSurface::MoveBy(const Vector3& difference, bool propagateUpdate)
	{
		std::unordered_set<Point*> pointsMoved;

		for (auto& patch : m_patches)
		{
			for (auto& point : patch->m_controlPoints)
			{
				if (pointsMoved.find(point) != pointsMoved.end())
				{
					continue;
				}

				pointsMoved.insert(point);
				point->MoveBy(difference, false);
			}
		}

		pointsMoved.clear();

		for (auto& patch : m_patches)
		{
			for (auto& point : patch->m_controlPoints)
			{
				if (pointsMoved.find(point) != pointsMoved.end())
				{
					continue;
				}

				pointsMoved.insert(point);
				point->OnUpdate();
			}
		}
	}

	void BezierSurface::OnDelete()
	{
		while (!m_patches.empty())
		{
			auto patch = *m_patches.begin();
			m_scene.RemoveSceneObject(patch->Id());
		}
	}

	void BezierSurface::OnDeletePatch(BezierPatch* patch)
	{
		auto result = std::find(m_patches.begin(), m_patches.end(), patch);
		m_patches.erase(result);

		if (m_patches.size() == 0)
		{
			m_scene.RemoveSceneObject(m_id);
		}
	}
	
	void BezierSurface::OnSelect()
	{
		for (auto& patch : m_patches)
		{
			for (auto& point : patch->m_controlPoints)
			{
				point->OnSelect();
			}
		}
	}
}
