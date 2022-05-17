#pragma once

#include "BezierSurface.h"

namespace BuD
{
	class BezierSurfaceC0 : public BezierSurface
	{
	public:
		BezierSurfaceC0(Scene& scene, const DX11Device& device,
			Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder = false);

		virtual void Accept(AbstractVisitor& visitor) override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::SURFACE_C0; }

		friend class GuiDrawer;
	};
}
