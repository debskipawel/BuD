#pragma once

#include <MathObjects/Parameterized2DObjectCPU.h>
#include <Objects/Abstract/PointBasedObject.h>

namespace BuD
{
	class TessellatedBezierSurface : public PointBasedObject
	{
	public:
		TessellatedBezierSurface(Scene& scene, const DX11Device& device,
			Vector3 position, float surfaceWidth, float surfaceLength, int patchesU, int patchesV, bool asCylinder);

		virtual Vector3 GetPoint(Vector2 uv) = 0;

		virtual void OnUpdate() = 0;

	protected:
		virtual void BuildCylinder(const DX11Device& device, Vector3 position, float surfaceWidth, float surfaceLength, int pointsU, int pointsV) = 0;
		virtual void CreateMesh(const DX11Device& device) = 0;

		int m_patchesU, m_patchesV;
		bool m_wrappedU, m_wrappedV;
	};
}
