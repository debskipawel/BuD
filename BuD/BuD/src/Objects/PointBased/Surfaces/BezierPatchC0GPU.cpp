#include "BezierPatchC0GPU.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>
#include <Objects/Scene.h>

#include <algorithm>
#include <iterator>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> polygonElements
	{
		{
			"POSITION0", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITIONA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"POSITIONB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"POSITIONC", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"POSITIOND", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	BezierPatchC0GPU::BezierPatchC0GPU(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurface* owner)
		: BezierPatch(scene, device, controlPoints, samplesU, samplesV, owner)
	{
		m_tag = "Patch C0";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/bezier_patch_c0_vs.hlsl", elements, { sizeof(Matrix) });
		auto polygonVertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device, L"../BuD/shaders/bezier_patch_c0_gs.hlsl", { sizeof(Matrix), sizeof(Vector4) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data(), D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto polygonVB = std::make_shared<DX11VertexBuffer>(device, m_controlPoints.size() * sizeof(Vector3), polygonElements);
		auto polygonIB = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 2 * m_controlPoints.size() * sizeof(unsigned short), nullptr, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		auto mesh = std::make_shared<Mesh>(vertexShader, geometryShader, pixelShader, vertexBuffer, indexBuffer,
			[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = view * projection;

				entity->GeometryShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			},
			[this](int run, Mesh* mesh)
			{
				int loopPasses = run * 42; // 252 vertices in each pass, 6 for every loop pass
				int startU = loopPasses % SamplesU();
				int startV = loopPasses / SamplesV();

				struct Params
				{
					int params[4];
				};

				Params params{ { SamplesU(), SamplesV(), startU, startV } };
				mesh->GeometryShader()->UpdateConstantBuffer(1, &params, sizeof(Params));

				return loopPasses >= SamplesU() * SamplesV();
			}
		);

		m_bezierPolygonMesh = std::make_shared<Mesh>(polygonVertexShader, nullptr, pixelShader, polygonVB, polygonIB,
			[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * view * projection;
				Vector3 polygonColor = { 1.0f, 0.0f, 0.0f };

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &polygonColor, sizeof(Vector3));
			}
		);

		m_meshes.push_back(mesh);

		OnUpdate();
	}

	void BezierPatchC0GPU::OnUpdate()
	{
		if (m_controlPoints.size() != 16)
		{
			m_scene.RemoveSceneObject(m_id);
			return;
		}

		Meshify();

		m_meshes[0]->VertexBuffer()->Update(m_vertices.data(), m_vertices.size() * sizeof(Vector3));
		m_meshes[0]->IndexBuffer()->Update(m_indices.data(), m_indices.size() * sizeof(unsigned short));

		std::vector<Vector3> polygonVertices(16);
		std::vector<unsigned short> polygonIndices;

		std::transform(m_controlPoints.begin(), m_controlPoints.end(), polygonVertices.begin(), [](SceneObject* obj) { return obj->Position(); });

		for (int i = 0; i < 16; i++)
		{
			if ((i + 1) % 4 == i % 4 + 1)
			{
				polygonIndices.push_back(i);
				polygonIndices.push_back(i + 1);
			}

			if (i + 4 < 16)
			{
				polygonIndices.push_back(i);
				polygonIndices.push_back(i + 4);
			}
		}

		m_bezierPolygonMesh->VertexBuffer()->Update(polygonVertices.data(), polygonVertices.size() * sizeof(Vector3));
		m_bezierPolygonMesh->IndexBuffer()->Update(polygonIndices.data(), polygonIndices.size() * sizeof(unsigned short));
	}
	
	void BezierPatchC0GPU::Meshify()
	{
		m_vertices.clear();
		m_vertices.resize(16);
		m_indices.clear();

		std::transform(m_controlPoints.begin(), m_controlPoints.end(), m_vertices.begin(), [](Point* point) { return point->Position(); });
		
		std::vector<unsigned short> polygonIndices;

		for (int i = 0; i < 16; i++)
		{
			m_indices.push_back(i);
		}
	}

	void BezierPatchC0GPU::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}
	
	Vector3 BezierPatchC0GPU::GetPoint(Vector2 parameter)
	{
		return Vector3();
	}
}
