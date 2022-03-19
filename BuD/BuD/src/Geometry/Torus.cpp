#include "Torus.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <imgui.h>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	Torus::Torus(const DX11Device& device, float largeRadius, float smallRadius)
		: Parameterized2DEntity({ 0.0f, 0.0f }, { 6.28318f, 6.28318f }, TorusEquation()),
		m_largeRadius(largeRadius), m_smallRadius(smallRadius)
	{
		UpdateSampleIntervals(8, 8);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_white_ps.hlsl");

		vertexShader->AddConstantBuffer(ConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data());

		m_model = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
			}
		);
	}

	std::shared_ptr<DX11ConstantBuffer> Torus::s_constantBuffer = nullptr;

	bool Torus::UpdateRadius(float largeRadius, float smallRadius)
	{
		if (m_largeRadius == largeRadius && m_smallRadius == smallRadius)
		{
			return false;
		}

		m_largeRadius = largeRadius;
		m_smallRadius = smallRadius;

		Sample();
		return true;
	}

	void Torus::DrawGui()
	{
		Parameterized2DEntity::DrawGui();

		float r = m_smallRadius;
		float R = m_largeRadius;

		ImGui::Text("Torus radius:");
		ImGui::DragFloat("Large", &R, 0.1f, 0.001f);
		ImGui::DragFloat("Small", &r, 0.1f, 0.001f, R);

		if (UpdateRadius(R, r))
		{
			UpdateRenderableModel();
		}
	}

	void Torus::UpdateRenderableModel()
	{
		m_model->VertexBuffer()->Update(m_vertices.data(), m_vertices.size() * sizeof(Vector3));
		m_model->IndexBuffer()->Update(m_indices.data(), m_indices.size() * sizeof(unsigned short));

		return;
	}

	std::shared_ptr<DX11ConstantBuffer> Torus::ConstantBuffer(const DX11Device& device)
	{
		if (!s_constantBuffer)
		{
			s_constantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_constantBuffer;
	}
}
