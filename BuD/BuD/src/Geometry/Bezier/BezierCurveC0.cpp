#include "BezierCurveC0.h"

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

	BezierCurveC0::BezierCurveC0(const DX11Device& device, std::vector<SceneObject*> controlPoints)
		: BezierCurve(controlPoints)
	{
		m_tag = "Bezier C0";
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

				std::vector<Vector3> controlPoints;
				std::vector<unsigned short> controlPointsIndices;
				
				controlPoints.reserve(controlPointsCount);
				controlPointsIndices.reserve(2 * controlPointsCount);

				for (int i = 0; i < controlPointsCount; i++)
				{
					auto& point = m_controlPoints[i];
					controlPoints.push_back(point->GetMesh(0)->m_position);
					controlPointsIndices.push_back(i);

					if (i && i % 3 == 0)
					{
						controlPointsIndices.push_back(i);
					}
				}

				auto extraIndices = controlPointsIndices.size() % 4 ? 4 - controlPointsIndices.size() % 4 : 0;

				for (int i = 0; i < extraIndices; i++)
				{
					controlPointsIndices.push_back(m_controlPoints.size() - 1);
				}

				entity->VertexBuffer()->Update(controlPoints.data(), controlPoints.size() * sizeof(Vector3));
				entity->IndexBuffer()->Update(controlPointsIndices.data(), controlPointsIndices.size() * sizeof(unsigned short));

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

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::VSConstantBuffer(const DX11Device& device)
	{
		if (!s_vsConstantBuffer)
		{
			s_vsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_vsConstantBuffer;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::GSConstantBuffer(const DX11Device& device)
	{
		if (!s_gsConstantBuffer)
		{
			s_gsConstantBuffer = std::make_shared<DX11ConstantBuffer>(device, sizeof(Matrix));
		}

		return s_gsConstantBuffer;
	}

	GeometryType BezierCurveC0::GetType()
	{
		return GeometryType::BEZIER_C0;
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
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::s_gsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC0::s_psConstantBuffer = nullptr;
}
