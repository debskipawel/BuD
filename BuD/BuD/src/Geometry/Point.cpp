#include "Point.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <imgui.h>

namespace BuD
{
	std::shared_ptr<BuD::DX11VertexBuffer> Point::s_vertexBuffer = nullptr;
	std::shared_ptr<BuD::DX11IndexBuffer> Point::s_indexBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> Point::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> Point::s_psConstantBuffer = nullptr;
	
	static std::vector<Vector3> vertices =
	{
		{ -0.03f, -0.03f, -0.03f }, 
		{ +0.03f, -0.03f, -0.03f }, 
		{ +0.03f, +0.03f, -0.03f }, 
		{ -0.03f, +0.03f, -0.03f }, 
			 		 		 
		{ -0.03f, -0.03f, +0.03f }, 
		{ +0.03f, +0.03f, +0.03f }, 
		{ +0.03f, -0.03f, +0.03f }, 
		{ -0.03f, +0.03f, +0.03f }, 
			 		 		 
		{ -0.03f, -0.03f, -0.03f }, 
		{ -0.03f, +0.03f, -0.03f }, 
		{ -0.03f, -0.03f, +0.03f }, 
		{ -0.03f, +0.03f, +0.03f }, 
			 		 		 
		{ +0.03f, -0.03f, -0.03f }, 
		{ +0.03f, +0.03f, -0.03f }, 
		{ +0.03f, -0.03f, +0.03f }, 
		{ +0.03f, +0.03f, +0.03f }, 
			 		 		 
		{ +0.03f, +0.03f, -0.03f }, 
		{ +0.03f, +0.03f, +0.03f }, 
		{ -0.03f, +0.03f, -0.03f }, 
		{ -0.03f, +0.03f, +0.03f }, 
			 		 		 
		{ +0.03f, -0.03f, -0.03f }, 
		{ +0.03f, -0.03f, +0.03f }, 
		{ -0.03f, -0.03f, -0.03f }, 
		{ -0.03f, -0.03f, +0.03f }, 
	};

	static std::vector<unsigned short> indices =
	{
		0,2,1, 0,3,2,
		4,6,5, 4,6,7,
		8,10,11, 8,9,11,
		12,14,15, 12,13,15,
		16,18,19, 16,17,19,
		20,22,23, 20,21,23
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	Point::Point(Vector3 position, const DX11Device& device)
		: SceneObject()
	{
		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_color_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		m_model = std::make_shared<Mesh>(vertexShader, pixelShader, GetVB(device), GetIB(device),
			[this](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		m_model->m_position = position;
	}

	void Point::DrawGui()
	{
		ImGui::Text("Position");
		ImGui::DragFloat("p(x)", &m_model->m_position.x);
		ImGui::DragFloat("p(y)", &m_model->m_position.y);
		ImGui::DragFloat("p(z)", &m_model->m_position.z);
	}

	std::shared_ptr<DX11ConstantBuffer> Point::VSConstantBuffer(const DX11Device& device)
	{
		if (!s_vsConstantBuffer)
		{
			s_vsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_vsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> Point::PSConstantBuffer(const DX11Device& device)
	{
		if (!s_psConstantBuffer)
		{
			s_psConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Vector4));
		}

		return s_psConstantBuffer;
	}

	std::shared_ptr<BuD::DX11VertexBuffer> Point::GetVB(const BuD::DX11Device& device)
	{
		if (!s_vertexBuffer)
		{
			s_vertexBuffer = std::make_shared<BuD::DX11VertexBuffer>(
				device,
				vertices.size() * sizeof(Vector3),
				elements,
				vertices.data()
				);
		}

		return s_vertexBuffer;
	}

	std::shared_ptr<BuD::DX11IndexBuffer> Point::GetIB(const BuD::DX11Device& device)
	{
		if (!s_indexBuffer)
		{
			s_indexBuffer = std::make_shared<BuD::DX11IndexBuffer>(
				device,
				DXGI_FORMAT_R16_UINT,
				indices.size() * sizeof(unsigned short),
				indices.data()
				);
		}

		return s_indexBuffer;
	}
}
