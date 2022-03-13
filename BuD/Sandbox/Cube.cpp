#include "Cube.h"

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct PositionNormal
{
	Vector3 position, normal;
};

static std::vector<PositionNormal> vertices =
{
	//Front Face
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
	{ { +0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
	{ { +0.5f, +0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
	{ { -0.5f, +0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
	//Back face
	{ { -0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f } },
	{ { +0.5f, +0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f } },
	{ { +0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, +0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f } },
	//Left face
	{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f } },
	{ { -0.5f, +0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, +0.5f }, { -1.0f, 0.0f, 0.0f } },
	{ { -0.5f, +0.5f, +0.5f }, { -1.0f, 0.0f, 0.0f } },
	//Right face
	{ { +0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ { +0.5f, +0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ { +0.5f, -0.5f, +0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ { +0.5f, +0.5f, +0.5f }, { 1.0f, 0.0f, 0.0f } },
	//Top face
	{ { +0.5f, +0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ { +0.5f, +0.5f, +0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ { -0.5f, +0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ { -0.5f, +0.5f, +0.5f }, { 0.0f, 1.0f, 0.0f } },
	//Bottom face
	{ { +0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f } },
	{ { +0.5f, -0.5f, +0.5f }, { 0.0f, -1.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f } },
	{ { -0.5f, -0.5f, +0.5f }, { 0.0f, -1.0f, 0.0f } },
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
	},
	{
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(PositionNormal, normal),
		D3D11_INPUT_PER_VERTEX_DATA, 0
	}
};

std::shared_ptr<BuD::DX11VertexBuffer> Cube::s_vertexBuffer = nullptr;
std::shared_ptr<BuD::DX11IndexBuffer> Cube::s_indexBuffer = nullptr;

std::shared_ptr<BuD::RenderableSceneEntity> Cube::GetEntity(const BuD::DX11Device& device)
{
	auto constantBuffer = std::make_shared<BuD::DX11ConstantBuffer>(device.Raw(), sizeof(DirectX::SimpleMath::Matrix));

	auto vertexShader = BuD::DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_normal_vs.hlsl", elements);
	auto pixelShader = BuD::DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_white_ps.hlsl");

	vertexShader->AddConstantBuffer(constantBuffer);

	return std::make_shared<BuD::RenderableSceneEntity>(
		vertexShader, pixelShader,
		GetVB(device), GetIB(device),
		[device](std::shared_ptr<BuD::AbstractCamera> camera, BuD::RenderableSceneEntity* entity)
		{
			auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

			entity->VertexShader()->UpdateConstantBuffer(device.Context().Get(), 0, &matrix, sizeof(DirectX::SimpleMath::Matrix));
		}
	);
}

std::shared_ptr<BuD::DX11VertexBuffer> Cube::GetVB(const BuD::DX11Device& device)
{
	if (!s_vertexBuffer)
	{
		s_vertexBuffer = std::make_shared<BuD::DX11VertexBuffer>(
			device.Raw(),
			vertices.size() * sizeof(PositionNormal),
			elements,
			vertices.data()
		);
	}

	return s_vertexBuffer;
}

std::shared_ptr<BuD::DX11IndexBuffer> Cube::GetIB(const BuD::DX11Device& device)
{
	if (!s_indexBuffer)
	{
		s_indexBuffer = std::make_shared<BuD::DX11IndexBuffer>(
			device.Raw(),
			DXGI_FORMAT_R16_UINT,
			indices.size() * sizeof(unsigned short),
			indices.data()
		);
	}

	return s_indexBuffer;
}
