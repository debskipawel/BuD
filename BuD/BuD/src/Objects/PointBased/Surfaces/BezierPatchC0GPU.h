#pragma once

#include "BezierPatch.h"

namespace BuD
{
	class BezierPatchC0GPU : public BezierPatch
	{
	public:
		BezierPatchC0GPU(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurface* owner = nullptr);

		virtual void OnUpdate() override;

		virtual void Meshify() override;

		virtual void Accept(AbstractVisitor& visitor) override;
		virtual Vector3 GetPoint(Vector2 parameter) override;
	};
}
