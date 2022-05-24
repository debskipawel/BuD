#pragma once

#include "BezierSurface.h"

namespace BuD
{
	class BezierSurfaceC2 : public BezierSurface
	{
	public:
		BezierSurfaceC2(Scene& scene, const DX11Device& device,
			Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder = false);

		BezierSurfaceC2(Scene& scene, const std::vector<BezierPatch*>& patches);

		virtual void Accept(AbstractVisitor& visitor) override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::SURFACE_C2; }

		friend class GuiDrawer;
		friend class ObjectSerializer;
	};
}
