#pragma once

#include "BezierPatchC0.h"

#include <Objects/Scene.h>

namespace BuD
{
	class BezierSurfaceC0 : public SceneObject
	{
	public:
		BezierSurfaceC0(const DX11Device& device, Scene& scene, 
			Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder = false);

		virtual void OnUpdate() override;
		virtual void OnDelete(Scene& scene) override;

		virtual void Accept(AbstractVisitor& visitor) override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::SURFACE_C0; }

	protected:
		std::vector<BezierPatchC0*> m_patches;
		bool m_showBezierPolygon = false;

		friend class GuiDrawer;
	};
}
