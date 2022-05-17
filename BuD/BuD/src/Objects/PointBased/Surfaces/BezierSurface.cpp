#include "BezierSurface.h"

#include <Objects/Scene.h>

namespace BuD
{
	BezierSurface::BezierSurface(Scene& scene)
		: SceneObject(scene)
	{
	}

	void BezierSurface::MoveTo(const Vector3& position, bool propagateUpdate)
	{
		auto diff = position - m_patches[0]->m_controlPoints[0]->Position();

		MoveBy(diff);
	}

	void BezierSurface::MoveBy(const Vector3& difference, bool propagateUpdate)
	{
		for (auto& patch : m_patches)
		{
			for (auto& point : patch->m_controlPoints)
			{
				point->MoveBy(difference, false);
			}
		}

		for (auto& patch : m_patches)
		{
			for (auto& point : patch->m_controlPoints)
			{
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
	}
}
