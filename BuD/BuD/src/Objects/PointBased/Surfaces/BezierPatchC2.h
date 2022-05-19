#pragma once

#include "BezierPatch.h"

#include <array>

namespace BuD
{
	class BezierSurfaceC2;

	class BezierPatchC2 : public BezierPatch
	{
	public:
		BezierPatchC2(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurface* owner = nullptr);

		virtual void Accept(AbstractVisitor& visitor) override;
		virtual void OnUpdate() override;

		virtual ObjectFlags GetFlags() override { return ObjectFlags::BEZIER_PATCH_C2; }

	protected:
		virtual Vector3 GetPoint(Vector2 param) override;

		static std::array<float, 4> DeBoor(float t);

		float m_prevU = -1.0f;
		std::array<float, 4> m_uDeBoor;

		friend class BezierSurfaceC2;
		friend class GuiDrawer;
	};
}
