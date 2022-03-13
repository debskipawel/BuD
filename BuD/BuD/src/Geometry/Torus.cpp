#include "Torus.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

namespace BuD
{
	std::shared_ptr<DX11ConstantBuffer> Torus::s_constantBuffer = nullptr;

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	void Torus::UpdateModel(const DX11Device& device)
	{
		if (m_model)
		{
			m_model->VertexBuffer()->Update(device.Context().Get(), m_vertices.data(), m_vertices.size() * sizeof(Vector3));
			m_model->IndexBuffer()->Update(device.Context().Get(), m_indices.data(), m_indices.size() * sizeof(unsigned short));

			return;
		}

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_white_ps.hlsl");

		vertexShader->AddConstantBuffer(ConstantBuffer(device.Raw()));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device.Raw(), m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device.Raw(), DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data());

		m_model = std::make_shared<RenderableSceneEntity>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[device](std::shared_ptr<AbstractCamera> camera, RenderableSceneEntity* entity)
			{
				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(device.Context().Get(), 0, &matrix, sizeof(Matrix));
			}
		);
	}

	std::shared_ptr<DX11ConstantBuffer> Torus::ConstantBuffer(ID3D11Device* device)
	{
		if (!s_constantBuffer)
		{
			s_constantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_constantBuffer;
	}
}
