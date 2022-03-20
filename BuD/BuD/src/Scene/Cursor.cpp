#include "Cursor.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <imgui.h>

namespace BuD
{
	struct Vertex
	{
		Vector3 position;
		Vector4 rgba;
	};

	static std::vector<Vertex> vertices =
	{
		{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.1f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.1f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3),
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	static std::vector<unsigned short> indices =
	{
		0, 1,
		0, 2,
		0, 3
	};

	Cursor::Cursor(Vector3 position, const DX11Device& device)
	{
		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_color_cursor_vs.hlsl", elements);
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/pos_color_cursor_ps.hlsl");

		auto cb = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		vertexShader->AddConstantBuffer(cb);

		auto vb = std::make_shared<BuD::DX11VertexBuffer>(device, vertices.size() * sizeof(Vertex), elements, vertices.data());
		auto ib = std::make_shared<BuD::DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		m_model = std::make_shared<Mesh>(vertexShader, pixelShader, vb, ib,
			[](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				auto dist = entity->m_position - camera->Position();
				auto scale = max(3.0f * logf(dist.Length()), 1.0f);

				entity->m_scale = { scale, scale, scale };

				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
			}
		);

		m_model->m_position = position;
	}
	
	void Cursor::DrawGui()
	{
		ImGui::Text("Position");
		ImGui::DragFloat("p(x)", &m_model->m_position.x, 0.01f);
		ImGui::DragFloat("p(y)", &m_model->m_position.y, 0.01f);
		ImGui::DragFloat("p(z)", &m_model->m_position.z, 0.01f);
	}

	std::shared_ptr<Cursor> Cursor::GetCursorAt(Vector3 position, const DX11Device& device)
	{
		if (!s_cursor)
		{
			s_cursor = std::shared_ptr<Cursor>(new Cursor(position, device));
		}

		s_cursor->m_model->m_position = position;

		return s_cursor;
	}

	std::shared_ptr<Cursor> Cursor::s_cursor = nullptr;
}
