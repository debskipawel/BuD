#include "TessellatedBezierSurface.h"

#include <Objects/Scene.h>

namespace BuD
{
	TessellatedBezierSurface::TessellatedBezierSurface(Scene& scene, const DX11Device& device,
		Vector3 position, float surfaceWidth, float surfaceLength, int patchesU, int patchesV, bool asCylinder)
		: PointBasedObject(scene, {}), m_patchesU(patchesU), m_patchesV(patchesV), m_wrappedU(asCylinder), m_wrappedV(false)
	{
		auto pointsU = asCylinder ? 3 * patchesU : 3 * patchesU + 1;
		auto pointsV = 3 * patchesV + 1;
		auto pointsCount = pointsU * pointsV;

		auto du = 1.0f / (3 * patchesU);
		auto dv = 1.0f / (3 * patchesV);

		auto dx = surfaceWidth / (3 * patchesU);
		auto dz = surfaceLength / (3 * patchesV);

		auto originPoint = position - Vector3(surfaceWidth / 2, 0.0f, surfaceLength / 2);

		if (asCylinder)
		{
			BuildCylinder(device, originPoint, surfaceWidth, surfaceLength, pointsU, pointsV);
		}
		else
		{
			std::vector<Point*> points;

			for (int i = 0; i < pointsV; i++)
			{
				for (int j = 0; j < pointsU; j++)
				{
					auto position = originPoint + Vector3(j * dx, 0.0f, i * dz);
					auto point = scene.CreatePoint(device, position);

					points.push_back(point.get());
				}
			}

			m_controlPoints = points;
		}

		CreateMesh();
		OnUpdate();
	}
}