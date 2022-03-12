#include "RenderableSceneEntity.h"

#include <SimpleMath.h>

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
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

    std::shared_ptr<RenderableSceneEntity> RenderableSceneEntity::Cube(ID3D11Device* device)
    {
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements
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

		auto res = std::shared_ptr<RenderableSceneEntity>(new RenderableSceneEntity());
		res->m_vertexBuffer = std::make_shared<DX11VertexBuffer>(device, vertices.size() * sizeof(Vector2), elements, vertices.data());
		res->m_indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data());
		res->m_vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_normal_vs.hlsl", elements);
		res->m_pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_white_ps.hlsl");

		return res;
    }
}
