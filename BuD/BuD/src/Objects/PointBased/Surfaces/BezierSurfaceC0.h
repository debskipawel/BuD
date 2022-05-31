#pragma once

#include "BezierSurface.h"

namespace BuD
{
	class BezierSurfaceC0 : public BezierSurface
	{
	public:
		BezierSurfaceC0(Scene& scene, const DX11Device& device,
			Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder = false);
		
		BezierSurfaceC0(Scene& scene, const std::vector<BezierPatch*>& patches, int patchesU, int patchesV, bool asCylinder);

		virtual void Accept(AbstractVisitor& visitor) override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::SURFACE_C0; }

	protected:
		int m_patchesU, m_patchesV;
		bool m_cylinder;

		friend class GuiDrawer;
		friend class ObjectSerializer;
	};
}
