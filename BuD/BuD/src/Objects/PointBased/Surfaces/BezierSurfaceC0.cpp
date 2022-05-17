#include "BezierSurfaceC0.h"

#include <imgui.h>

namespace BuD
{
	BezierSurfaceC0::BezierSurfaceC0(const DX11Device& device, Scene& scene,
		Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder)
	{
		m_tag = "Bezier surface C0";

		if (asCylinder)
		{
			float alpha = DirectX::XM_2PI / patchesU;
			float ca = cosf(alpha), sa = sinf(alpha);

			float sideLength = 4.0f / 3 * tanf(alpha / 4);

			Vector3 versor = { 1.0f, 0.0f, 0.0f };

			auto size = 3 * patchesU;
			std::vector<Vector3> pointsPositions(size);

			for (int i = 0; i < patchesU; i++)
			{
				int index = 3 * i;
				
				Vector3 orth = versor.Cross(Vector3{ 0.0f, 0.0f, 1.0f });

				int prevIndex = index - 1 < 0 ? index - 1 + size : index - 1;
				pointsPositions[prevIndex] = position + patchWidth * (versor + orth * sideLength);

				pointsPositions[index] = position + patchWidth * versor;
				pointsPositions[index + 1] = position + patchWidth * (versor - orth * sideLength);

				versor = { ca * versor.x - sa * versor.y, sa * versor.x + ca * versor.y, 0.0f };
			}

			std::vector<Point*> points;
			points.reserve((3 * patchesU) * (3 * patchesV + 1));

			for (int j = 0; j < patchesV * 3 + 1; j++)
			{
				float z = position.z + j * (patchLength / 3) - (patchesV * patchLength / 2);

				for (int i = 0; i < patchesU * 3; i++)
				{
					auto point = scene.CreatePoint(device, pointsPositions[i] + Vector3(0.0f, 0.0f, z));
					points.push_back(point.get());
				}
			}

			for (int j = 0; j < patchesV; j++)
			{
				for (int i = 0; i < patchesU; i++)
				{
					std::vector<Point*> patchPoints(16);
					
					int startingU = 3 * i;
					int startingV = 3 * j;

					for (int p = 0; p < 16; p++)
					{
						int dv = p / 4;
						int du = p % 4;

						int indexU = (startingU + du) % (3 * patchesU);

						patchPoints[p] = points[(startingV + dv) * (3 * patchesU) + indexU];
					}

					auto patch = scene.CreateBezierPatchC0(device, patchPoints, sampleU, sampleV);
					m_patches.push_back(reinterpret_cast<BezierPatchC0*>(patch.get()));
				}
			}
		}
		else
		{
			std::vector<Point*> points;
			points.reserve((3 * patchesU + 1) * (3 * patchesV + 1));

			for (int i = 0; i < patchesU * 3 + 1; i++)
			{
				for (int j = 0; j < patchesV * 3 + 1; j++)
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

						int indexU = i * 3 + u;
						int indexV = j * 3 + v;

						patchPoints.push_back(points[indexU * (patchesV * 3 + 1) + indexV]);
					}

					auto patch = scene.CreateBezierPatchC0(device, patchPoints, sampleU, sampleV);

					m_patches.push_back(reinterpret_cast<BezierPatchC0*>(patch.get()));
				}
			}
		}
	}

	void BezierSurfaceC0::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}

	void BezierSurfaceC0::OnUpdate()
	{
	}
	
	void BezierSurfaceC0::OnDelete(Scene& scene)
	{
		for (auto& patch : m_patches)
		{
			scene.RemoveSceneObject(patch->Id());
		}
	}
}
