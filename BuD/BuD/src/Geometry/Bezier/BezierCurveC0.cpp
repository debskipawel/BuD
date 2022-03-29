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
				std::vector<SceneObject*> filteredControlPoints;
				filteredControlPoints.reserve(m_controlPoints.size());

				std::copy_if(m_controlPoints.begin(), m_controlPoints.end(), std::back_inserter(filteredControlPoints), [](SceneObject* obj) { return !obj->ShouldBeDeleted(); });
				m_controlPoints = filteredControlPoints;
				
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

				UINT minX = device.BufferWidth(), maxX = 0;
				UINT minY = device.BufferHeight(), maxY = 0;

				for (auto& cp : m_controlPoints)
				{
					auto position = cp->GetMesh(0)->m_position;
					auto trPosition = Vector4{ position.x, position.y, position.z, 1.0f };
					DirectX::XMStoreFloat4(&trPosition, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&trPosition), DirectX::XMLoadFloat4x4(&matrix)));
					trPosition /= trPosition.w;

					int xPos = device.BufferWidth() * (std::clamp(trPosition.x, -1.0f, 1.0f) + 1.0f) / 2.0f;
					int yPos = device.BufferHeight() * (std::clamp(trPosition.y, -1.0f, 1.0f) + 1.0f) / 2.0f;

					maxX = max(xPos, maxX);
					minX = min(xPos, minX);
					maxY = max(yPos, maxY);
					minY = min(yPos, minY);
				}

				auto longSide = max(maxX - minX, maxY - minY);

				// TODO: calculate sampleCount and send it through to the GS
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
