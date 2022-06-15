#include "GregoryPatch.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>
#include <Objects/Scene.h>

#include <Algorithms/GregoryPatchesCalculator.h>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	GregoryPatch::GregoryPatch(Scene& scene, const DX11Device& device, const std::vector<Point*>& cycleFirstLine, const std::vector<Point*>& cycleSecondLine)
		: PointBasedObject(scene, cycleFirstLine), m_secondLineControlPoints(cycleSecondLine), m_drawPolygon(false)
	{
		m_tag = "Gregory patch";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });
		auto deBoorPixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_light_blue_ps.hlsl", {});

		auto indices = Indices(m_samplesU, m_samplesV);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data());

		std::vector<unsigned short> polygonIndices =
		{
			0, 1, 1, 2, 2, 3, 3, 9, 9, 15, 15, 19, 19, 18, 18, 17, 17, 16, 16, 10, 10, 4, 4, 0,
			1, 6, 2, 7, 4, 5, 10, 11, 9, 8, 17, 12, 18, 13, 15, 14
		};
		auto polygonIndexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT,
			polygonIndices.size() * sizeof(unsigned short), polygonIndices.data(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		auto patchesCount = cycleFirstLine.size() / 3;

		for (int i = 0; i < patchesCount; i++)
		{
			auto vb = std::make_shared<DX11VertexBuffer>(device, m_samplesU * m_samplesV * sizeof(Vector3), elements);
			auto mesh = std::make_shared<Mesh>(vertexShader, nullptr, pixelShader, vb, indexBuffer,
				[this](const Matrix& view, const Matrix& projection, Mesh* mesh)
				{
					auto matrix = view * projection;

					mesh->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
					mesh->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
				}
			);

			m_meshes.push_back(mesh);
		}

		for (int i = 0; i < patchesCount; i++)
		{
			auto vb = std::make_shared<DX11VertexBuffer>(device, 20 * sizeof(Vector3), elements);
			auto mesh = std::make_shared<Mesh>(vertexShader, nullptr, pixelShader, vb, polygonIndexBuffer,
				[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
				{
					auto matrix = entity->GetModelMatrix() * view * projection;
					Vector3 polygonColor = { 1.0f, 0.0f, 0.0f };

					entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
					entity->PixelShader()->UpdateConstantBuffer(0, &polygonColor, sizeof(Vector3));
				}
			);

			m_polygonMeshes.push_back(mesh);
		}

		OnUpdate();
	}
	
	void GregoryPatch::OnUpdate()
	{
		auto patches = GregoryPatchCalculator::CalculateControlPoints(m_controlPoints, m_secondLineControlPoints);

		auto indices = Indices(m_samplesU, m_samplesV);
		m_meshes[0]->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));

		for (int i = 0; i < patches.size(); i++)
		{
			auto vertices = Meshify(patches[i]);
			m_polygonMeshes[i]->VertexBuffer()->Update(patches[i].data(), patches[i].size() * sizeof(Vector3));
			m_meshes[i]->VertexBuffer()->Update(vertices.data(), vertices.size() * sizeof(Vector3));
		}
	}
	
	void GregoryPatch::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}
	
	void GregoryPatch::OnSelect()
	{
		PointBasedObject::OnSelect();
		m_color = { 0.7f, 0.5f, 0.0f };
	}
	
	void GregoryPatch::OnUnselect()
	{
		PointBasedObject::OnUnselect();
		m_color = { 1.0f, 1.0f, 1.0f };
	}
	
	std::vector<Vector3> GregoryPatch::Meshify(const std::vector<Vector3>& cP)
	{
		float u = 0.0f, v = 0.0f;
		float du = 1.0f / (m_samplesU - 1), dv = 1.0f / (m_samplesV - 1);

		std::vector<Vector3> vertices;

		for (int i = 0; i < m_samplesV; i++)
		{
			u = 0.0f;

			auto bernsteinV = DeCasteljau(v);
			Vector4 BV = { bernsteinV[0], bernsteinV[1], bernsteinV[2], bernsteinV[3] };

			for (int j = 0; j < m_samplesU; j++)
			{
				auto bernsteinU = DeCasteljau(u);
				Vector4 BU = { bernsteinU[0], bernsteinU[1], bernsteinU[2], bernsteinU[3] };

				Vector3 pointOnPatch = {};

				for (int k = 0; k < 3; k++)
				{
					float F[4] = {};

					F[0] = (u * cP[5][k] + v * cP[5][k]) / (u + v + 0.001);
					F[1] = ((1 - u) * cP[11][k] + v * cP[12][k]) / (1 - u + v + 0.001);
					F[2] = ((1 - u) * cP[14][k] + (1 - v) * cP[13][k]) / (2 - u - v + 0.001);
					F[3] = (u * cP[8][k] + (1 - v) * cP[7][k]) / (1 + u - v + 0.001);

					Matrix G =
					{
						cP[0][k], cP[4][k], cP[10][k], cP[16][k],
						cP[1][k], F[0], F[1], cP[17][k],
						cP[2][k], F[3], F[2], cP[18][k],
						cP[3][k], cP[9][k], cP[15][k], cP[19][k]
					};

					pointOnPatch.Set(k, Vector4::Transform(BV, G).Dot(BU));
				}

				vertices.push_back(pointOnPatch);

				u += du;
			}

			v += dv;
		}

		return vertices;
	}

	void GregoryPatch::TogglePolygonDisplay(bool enable)
	{
		m_drawPolygon = enable;

		for (auto& mesh : m_polygonMeshes)
		{
			if (!enable)
			{
				m_meshes.erase(std::find(m_meshes.begin(), m_meshes.end(), mesh));
			}
			else
			{
				m_meshes.push_back(mesh);
			}
		}
	}

	std::array<float, 4> GregoryPatch::DeCasteljau(float t)
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
	
	std::vector<unsigned short> GregoryPatch::Indices(float samplesU, float samplesV)
	{
		std::vector<unsigned short> indices = {};

		for (int i = 0; i < samplesV - 1; i++)
		{
			for (int j = 0; j < samplesU - 1; j++)
			{
				indices.push_back(i * samplesU + j);
				indices.push_back(i * samplesU + (j + 1));
				indices.push_back((i + 1) * samplesU + j);
				indices.push_back(i * samplesU + j);
				indices.push_back((i + 1) * samplesU + j);
				indices.push_back((i + 1) * samplesU + j + 1);
			}
		}

		return indices;
	}
}
