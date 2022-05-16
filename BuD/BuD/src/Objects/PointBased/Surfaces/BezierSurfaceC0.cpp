#include "BezierSurfaceC0.h"

#include <imgui.h>

namespace BuD
{
	BezierSurfaceC0::BezierSurfaceC0(const DX11Device& device, Scene& scene,
		Vector3 position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV)
	{
		m_tag = "Bezier surface C0";

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

	void BezierSurfaceC0::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}

	void BezierSurfaceC0::OnUpdate()
	{
	}
	
	void BezierSurfaceC0::OnDelete()
	{
		for (auto& patch : m_patches)
		{
			patch->OnDelete();
		}
	}
}
