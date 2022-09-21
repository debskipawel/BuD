#pragma once

#include "TessellatedBezierSurface.h"

namespace BuD
{
	class TessellatedBezierSurfaceC0 : public TessellatedBezierSurface
	{
	public:
		TessellatedBezierSurfaceC0(Scene& scene, const DX11Device& device,
			Vector3 position, float surfaceWidth, float surfaceLength, int patchesU, int patchesV, bool asCylinder);

		virtual Vector3 GetPoint(Vector2 uv) override;

		virtual void OnUpdate() override;

	protected:
		virtual void BuildCylinder(const DX11Device& device, Vector3 position, float surfaceWidth, float surfaceLength, int pointsU, int pointsV) override;
		virtual void CreateMesh(const DX11Device& device) override;

		virtual void Accept(AbstractVisitor& visitor) override;
	};
}
