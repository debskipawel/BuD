#include "BezierCurveC2.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <imgui.h>
#include <algorithm>
#include <iterator>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	BezierCurveC2::BezierCurveC2(const DX11Device& device, std::vector<SceneObject*> controlPoints)
		: BezierCurve(controlPoints)
	{
		m_tag = "Bezier C2";
		m_controlPoints.reserve(16);
		m_meshes.reserve(1);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device.Raw(), L"../BuD/shaders/bezier_curve_c0_gs.hlsl");
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/bezier_with_polygon_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		geometryShader->AddConstantBuffer(GSConstantBuffer(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this, device](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				FilterControlPoints();
				UpdateCentroid();

				int controlPointsCount = m_controlPoints.size();

				if (controlPointsCount < 4)
				{
					entity->VertexBuffer()->Update(nullptr, 0);
					entity->IndexBuffer()->Update(nullptr, 0);

					return;
				}

				std::vector<Vector3> vertices;
				std::vector<unsigned short> indices;
				vertices.reserve(3 * controlPointsCount);
				indices.reserve((controlPointsCount - 3) * 4);

				for (int i = 0; i < controlPointsCount - 1; i++)
				{
					auto& a = m_controlPoints[i]->GetMesh(0)->m_position;
					auto& b = m_controlPoints[i + 1]->GetMesh(0)->m_position;

					vertices.push_back(a);
					vertices.push_back((2.0f * a + b) / 3.0f);
					vertices.push_back((a + 2.0f * b) / 3.0f);
				}

				for (int i = 1; i < controlPointsCount - 1; i++)
				{
					vertices[3 * i] = (vertices[3 * i - 1] + vertices[3 * i + 1]) / 2.0f;
				}

				for (int i = 3; i < vertices.size() - 2; i++)
				{
					indices.push_back(i - 3);

					if (indices.size() % 4 == 0)
					{
						indices.push_back(i - 3);
					}
				}

				entity->VertexBuffer()->Update(vertices.data() + 3, (vertices.size() - 5) * sizeof(Vector3));
				entity->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));
				
				auto matrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));

				struct GSResource
				{
					uint32_t samples;
					uint32_t drawPolygon;
				};

				auto rect = GetSurroundingRectangle(camera, device.BufferWidth(), device.BufferHeight());
				auto longSide = max(rect.right - rect.left, rect.bottom - rect.top);

				GSResource resource = { longSide / 10, m_drawPolygon };
				entity->GeometryShader()->UpdateConstantBuffer(0, &resource, sizeof(GSResource));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::VSConstantBuffer(const DX11Device& device)
	{
		if (!s_vsConstantBuffer)
		{
			s_vsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_vsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::GSConstantBuffer(const DX11Device& device)
	{
		if (!s_gsConstantBuffer)
		{
			s_gsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_gsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::PSConstantBuffer(const DX11Device& device)
	{
		if (!s_psConstantBuffer)
		{
			s_psConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Vector4));
		}

		return s_psConstantBuffer;
	}

	GeometryType BezierCurveC2::GetType()
	{
		return GeometryType::BEZIER_C2;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_gsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_psConstantBuffer = nullptr;
}
