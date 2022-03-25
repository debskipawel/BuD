#include "BezierCurveC0.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

namespace BuD
{
	static std::vector<Vector3> vertices =
	{
		{ -1.0f, -0.5f, -2.0f },
		{ -0.5f,  0.5f, -2.0f },
		{  0.5f,  0.5f, -2.0f },
		{  1.0f, -0.5f, -2.0f },
	};

	static std::vector<unsigned short> indices =
	{
		0, 1, 2, 3
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	BezierCurveC0::BezierCurveC0(const DX11Device& device)
		: m_color(1.0f, 1.0f, 1.0f)
	{
		m_tag = "Bezier C0";
		m_meshes.reserve(1);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device.Raw(), L"../BuD/shaders/bezier_curve_c0_gs.hlsl");
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_color_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, vertices.size() * sizeof(Vector3), elements, vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::VSConstantBuffer(const DX11Device& device)
	{
		if (!s_vsConstantBuffer)
		{
			s_vsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_vsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::PSConstantBuffer(const DX11Device& device)
	{
		if (!s_psConstantBuffer)
		{
			s_psConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Vector4));
		}

		return s_psConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::s_psConstantBuffer = nullptr;
}
