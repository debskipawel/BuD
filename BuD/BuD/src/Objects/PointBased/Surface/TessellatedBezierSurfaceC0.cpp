#include "TessellatedBezierSurfaceC0.h"

#include <array>

#include "Objects/Scene.h"
#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

namespace BuD
{
	std::array<float, 4> DeCasteljau(float t);

	TessellatedBezierSurfaceC0::TessellatedBezierSurfaceC0(Scene& scene, const DX11Device& device,
		Vector3 position, float surfaceWidth, float surfaceLength, int patchesU, int patchesV, bool asCylinder)
		: TessellatedBezierSurface(scene, device, position, surfaceWidth, surfaceLength, patchesU, patchesV, asCylinder)
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

		CreateMesh(device);
		OnUpdate();

		m_tag = "Tessellated Bezier surface C0";
	}

	Vector3 TessellatedBezierSurfaceC0::GetPoint(Vector2 uv)
	{
		auto u = uv.x;
		auto v = uv.y;
		auto patchU = (int)(u * m_patchesU);
		auto patchV = (int)(v * m_patchesV);

		auto col = 3 * patchU;
		auto row = 3 * patchV;

		auto pointsU = m_wrappedU ? m_patchesU * 3 : m_patchesU * 3 + 1;
		auto pointsV = m_wrappedV ? m_patchesV * 3 : m_patchesV * 3 + 1;

		auto uDeCasteljau = DeCasteljau(u);
		auto vDeCasteljau = DeCasteljau(v);

		Vector3 result = {};

		for (int i = 0; i < 4; i++)
		{
			auto currentRow = row + i;

			Vector3 partResult = {};

			for (int j = 0; j < 4; j++)
			{
				auto currentCol = col + j;

				auto index = currentRow * pointsU + currentCol;

				auto point = m_controlPoints[index]->Position();
				partResult += point * uDeCasteljau[j];
			}

			result += partResult * vDeCasteljau[i];
		}

		return result;
	}

	void TessellatedBezierSurfaceC0::OnUpdate()
	{
		auto pointsU = m_wrappedU ? m_patchesU * 3 : m_patchesU * 3 + 1;
		auto pointsV = m_wrappedV ? m_patchesV * 3 : m_patchesV * 3 + 1;

		std::vector<float> vertices;
		vertices.reserve(5 * pointsU * pointsV);

		for (int i = 0; i < pointsV; i++)
		{
			for (int j = 0; j < pointsU; j++)
			{
				auto index = i * pointsU + j;

				auto position = m_controlPoints[index]->Position();
				auto u = (float)j / (m_patchesU * 3);
				auto v = (float)i / (m_patchesV * 3);

				vertices.push_back(position.x);
				vertices.push_back(position.y);
				vertices.push_back(position.z);
				vertices.push_back(u);
				vertices.push_back(v);
			}
		}

		m_meshes[0]->VertexBuffer()->Update(vertices.data(), vertices.size() * sizeof(float));

		std::vector<unsigned short> indices;
		indices.reserve(m_patchesU * m_patchesV * 16);

		for (int i = 0; i < m_patchesV; i++)
		{
			for (int j = 0; j < m_patchesV; j++)
			{
				for (int v = 0; v < 4; v++)
				{
					for (int u = 0; u < 4; u++)
					{
						auto row = (3 * j + v) % pointsV;
						auto col = (3 * i + u) % pointsU;

						indices.push_back(row * pointsU + col);
					}
				}
			}
		}

		m_meshes[0]->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));
	}

	void TessellatedBezierSurfaceC0::BuildCylinder(const DX11Device& device, Vector3 position, float surfaceWidth, float surfaceLength, int pointsU, int pointsV)
	{

	}

	void TessellatedBezierSurfaceC0::CreateMesh(const DX11Device& device)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements
		{
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"PARAMS", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 3 * sizeof(float),
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/local_to_view_vs.hlsl", elements, { 2 * sizeof(Matrix) });
		auto hullShader = DX11ShaderLoader::Get()->HSLoad(device, L"../BuD/shaders/bezier_patch_hs.hlsl");
		auto domainShader = DX11ShaderLoader::Get()->DSLoad(device, L"../BuD/shaders/bezier_patch_c0_ds.hlsl", { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_white_ps.hlsl");

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_controlPoints.size() * sizeof(float) * 5, elements);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_controlPoints.size() * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[](const Matrix& view, const Matrix& projection, Mesh* self)
			{
				Matrix vcb[] = { Matrix::Identity, view };

				self->VertexShader()->UpdateConstantBuffer(0, vcb, 2 * sizeof(Matrix));
				self->DomainShader()->UpdateConstantBuffer(0, &projection, sizeof(Matrix));
			});

		mesh->SetHS(hullShader);
		mesh->SetDS(domainShader);

		m_meshes.push_back(mesh);
	}

	void TessellatedBezierSurfaceC0::Accept(AbstractVisitor& visitor)
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
