#pragma once

#include "BezierPatchC0.h"

namespace BuD
{
	class BezierSurfaceC0 : public SceneObject
	{
	public:
		BezierSurfaceC0(Scene& scene, const DX11Device& device,
			Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder = false);

		virtual void OnUpdate() override;
		virtual void OnDelete() override;
		virtual void OnDeletePatch(BezierPatchC0* patch);

		virtual void Accept(AbstractVisitor& visitor) override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::SURFACE_C0; }

	protected:
		std::vector<BezierPatchC0*> m_patches;
		bool m_showBezierPolygon = false;

		friend class GuiDrawer;
	};
}
