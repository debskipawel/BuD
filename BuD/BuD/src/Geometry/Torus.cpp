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
		m_largeRadius(largeRadius), m_smallRadius(smallRadius), m_color(1.0f, 1.0f, 1.0f)
	{
		UpdateSampleIntervals(8, 8);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_color_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data());

		m_mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);
	}

	std::shared_ptr<DX11ConstantBuffer> Torus::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> Torus::s_psConstantBuffer = nullptr;

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
		ImGui::Text("Rotation");
		ImGui::DragFloat("r(x)", &m_mesh->m_rotation.x, 1.0f);
		ImGui::DragFloat("r(y)", &m_mesh->m_rotation.y, 1.0f);
		ImGui::DragFloat("r(z)", &m_mesh->m_rotation.z, 1.0f);
		ImGui::NewLine();
		
		ImGui::Text("Scale");
		ImGui::DragFloat("s(x)", &m_mesh->m_scale.x, 0.1f);
		ImGui::DragFloat("s(y)", &m_mesh->m_scale.y, 0.1f);
		ImGui::DragFloat("s(z)", &m_mesh->m_scale.z, 0.1f);
		ImGui::NewLine();
		
		ImGui::Text("Position");
		ImGui::DragFloat("p(x)", &m_mesh->m_position.x, 0.5f);
		ImGui::DragFloat("p(y)", &m_mesh->m_position.y, 0.5f);
		ImGui::DragFloat("p(z)", &m_mesh->m_position.z, 0.5f);
		ImGui::NewLine();

		Parameterized2DEntity::DrawGui();

		float r = m_smallRadius;
		float R = m_largeRadius;

		const float minSmallRadius = 0.001f;
		const float minLargeRadius = 0.001f;
		const float maxSmallRadius = m_largeRadius;

		ImGui::Text("Torus radius");
		ImGui::DragFloat("Large", &R, 0.1f, 0.001f);
		ImGui::DragFloat("Small", &r, 0.1f, 0.001f, R);

		R = R < minLargeRadius ? minLargeRadius : R;
		r = r < minSmallRadius ? minSmallRadius : r;
		r = r > maxSmallRadius ? maxSmallRadius : r;

		if (UpdateRadius(R, r))
		{
			UpdateRenderableModel();
		}
	}

	void Torus::UpdateRenderableModel()
	{
		m_mesh->VertexBuffer()->Update(m_vertices.data(), m_vertices.size() * sizeof(Vector3));
		m_mesh->IndexBuffer()->Update(m_indices.data(), m_indices.size() * sizeof(unsigned short));

		return;
	}

	std::shared_ptr<DX11ConstantBuffer> Torus::VSConstantBuffer(const DX11Device& device)
	{
		if (!s_vsConstantBuffer)
		{
			s_vsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_vsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> Torus::PSConstantBuffer(const DX11Device& device)
	{
		if (!s_psConstantBuffer)
		{
			s_psConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Vector4));
		}

		return s_psConstantBuffer;
	}
}
