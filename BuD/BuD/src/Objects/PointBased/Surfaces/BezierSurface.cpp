#include "BezierSurface.h"

#include <Objects/Scene.h>

namespace BuD
{
	BezierSurface::BezierSurface(Scene& scene)
		: SceneObject(scene)
	{
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
