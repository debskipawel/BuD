#pragma once

#include "BezierPatch.h"

namespace BuD
{
	class BezierSurface : public SceneObject
	{
	public:
		BezierSurface(Scene& scene);

		virtual void MoveTo(const Vector3& position, bool propagateUpdate = false) override;
		virtual void MoveBy(const Vector3& difference, bool propagateUpdate = false) override;
		//virtual void ScaleTo(const Vector3& scale, bool propagateUpdate = false) override;
		//virtual void RotateTo(const Quaternion& quaternion, bool propagateUpdate = false) override;
		//virtual void RotateTo(const Vector3& rotation, bool propagateUpdate = false) override;

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
