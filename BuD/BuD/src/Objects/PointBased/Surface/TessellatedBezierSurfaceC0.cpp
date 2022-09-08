#include "TessellatedBezierSurfaceC0.h"

#include <array>

#include "Objects/Scene.h"

namespace BuD
{
	std::array<float, 4> DeCasteljau(float t);

	TessellatedBezierSurfaceC0::TessellatedBezierSurfaceC0(Scene& scene, const DX11Device& device,
		Vector3 position, float surfaceWidth, float surfaceLength, int patchesU, int patchesV, bool asCylinder)
		: TessellatedBezierSurface(scene, device, position, surfaceWidth, surfaceLength, patchesU, patchesV, asCylinder)
	{
	}

	Vector3 TessellatedBezierSurfaceC0::GetPoint(Vector2 uv)
	{
		auto u = uv.x;
		auto v = uv.y;
		auto patchU = (int)(u * m_patchesU);
		auto patchV = (int)(v * m_patchesV);

		auto col = 3 * patchU;
		auto row = 3 * patchV;

		auto pointsU = m_wrappedU ? m_patchesU * 3 + 1 : m_patchesU * 3;
		auto pointsV = m_wrappedV ? m_patchesV * 3 + 1 : m_patchesV * 3;

		auto uDeCasteljau = DeCasteljau(u);
		auto vDeCasteljau = DeCasteljau(v);

		Vector3 result = {};

		for (int i = 0; i < 4; i++)
		{
			Vector3 partResult = {};

			for (int j = 0; j < 4; j++)
			{
				auto point = m_controlPoints[v * 4 + u]->Position();
				partResult += point * vDeCasteljau[v];
			}

			result += partResult * uDeCasteljau[u];
		}

		return result;
	}

	void TessellatedBezierSurfaceC0::BuildCylinder(const DX11Device& device, Vector3 position, float surfaceWidth, float surfaceLength, int pointsU, int pointsV)
	{

	}

	void TessellatedBezierSurfaceC0::CreateMesh()
	{

	}

	std::array<float, 4> DeCasteljau(float t)
	{
		auto res = std::array<float, 4> { 0.0f };
		res[0] = 1.0f;

		float u = 1.0f - t;

		for (int j = 1; j <= 3; j++)
		{
			for (int i = j; i >= 1; i--)
			{
				res[i] = res[i] * u + res[i - 1] * t;
			}

			res[0] = res[0] * u;
		}

		return res;
	}
}
