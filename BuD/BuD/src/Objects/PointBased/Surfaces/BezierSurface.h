#pragma once

#include "BezierPatch.h"

namespace BuD
{
	class BezierSurface : public SceneObject
	{
	public:
		BezierSurface(Scene& scene);

		virtual void OnUpdate() override {}
		virtual void OnDelete() override;
		virtual void OnDeletePatch(BezierPatch* patch);

		virtual ObjectFlags GetFlags() override { return ObjectFlags::SURFACE; }

	protected:
		std::vector<BezierPatch*> m_patches;
		bool m_showBezierPolygon = false;

		friend class GuiDrawer;
	};
}
