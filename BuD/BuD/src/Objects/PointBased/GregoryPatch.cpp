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
		: PointBasedObject(scene, cycleFirstLine), m_secondLineControlPoints(cycleSecondLine)
	{
		m_tag = "Gregory patch";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });
		auto deBoorPixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_light_blue_ps.hlsl", {});

		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_samplesU * m_samplesV * 6);

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

		OnUpdate();
	}
	
	void GregoryPatch::OnUpdate()
	{
		auto patches = GregoryPatchCalculator::CalculateControlPoints(m_controlPoints, m_secondLineControlPoints);

		for (int i = 0; i < patches.size(); i++)
		{
			auto vertices = Meshify(patches[i]);
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
	
	std::vector<Vector3> GregoryPatch::Meshify(const std::vector<Vector3>& controlPoints)
	{
		float u = 0.0f, v = 0.0f;
		float du = 1.0f / (m_samplesU - 1), dv = 1.0f / (m_samplesV - 1);

		std::vector<Vector3> vertices;

		for (int i = 0; i < m_samplesV; i++)
		{
			u = 0.0f;

			for (int j = 0; j < m_samplesU; j++)
			{
				// calculate point in (u,v)

				u += du;
			}

			v += dv;
		}

		return vertices;
	}
}
