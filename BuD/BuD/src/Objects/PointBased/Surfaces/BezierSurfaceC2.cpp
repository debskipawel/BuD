#include "BezierSurfaceC2.h"

#include <Objects/Scene.h>

namespace BuD
{
	BezierSurfaceC2::BezierSurfaceC2(Scene& scene, const DX11Device& device, Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder)
		: BezierSurface(scene)
	{
		m_tag = "Bezier surface C2";

		if (asCylinder)
		{

		}
		else
		{
			std::vector<Point*> points;
			points.reserve((patchesU + 3) * (patchesV + 3));

			for (int i = 0; i < patchesU + 3; i++)
			{
				for (int j = 0; j < patchesV + 3; j++)
				{
					auto point = scene.CreatePoint(device, position + Vector3(0.0f, 0.0f, i * patchWidth / 3) + Vector3(j * patchLength / 3, 0.0f, 0.0f));
					points.push_back(point.get());
				}
			}

			for (int i = 0; i < patchesU; i++)
			{
				for (int j = 0; j < patchesV; j++)
				{
					std::vector<Point*> patchPoints;
					patchPoints.reserve(16);

					for (int p = 0; p < 16; p++)
					{
						int u = p / 4;
						int v = p % 4;

						int indexU = i + u;
						int indexV = j + v;

						patchPoints.push_back(points[indexU * (patchesV + 3) + indexV]);
					}

					auto patch = scene.CreateBezierPatchC2(device, patchPoints, sampleU, sampleV, this);

					m_patches.push_back(reinterpret_cast<BezierPatch*>(patch.get()));
				}
			}
		}
	}
	
	void BezierSurfaceC2::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}
}
