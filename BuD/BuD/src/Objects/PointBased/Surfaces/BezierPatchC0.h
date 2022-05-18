#pragma once

#include "BezierPatch.h"

#include <array>

namespace BuD
{
	class BezierSurfaceC0;

	class BezierPatchC0 : public BezierPatch
	{
	public:
		BezierPatchC0(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurface* owner = nullptr);

		virtual void Accept(AbstractVisitor& visitor) override;
		virtual void OnUpdate() override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::BEZIER_PATCH_C0; }

	protected:
		virtual Vector3 GetPoint(Vector2 param) override;

		static std::array<float, 4> DeCasteljau(float t);
		
		float m_prevU = -1.0f;
		std::array<float, 4> m_uDeCasteljau;

		friend class BezierSurfaceC0;
		friend class GuiDrawer;
	};
}
