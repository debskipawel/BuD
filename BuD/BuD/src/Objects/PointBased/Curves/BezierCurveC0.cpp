#include "BezierCurveC0.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

#include <Objects/Independent/Point.h>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	BezierCurveC0::BezierCurveC0(const DX11Device& device, const std::vector<Point*>& controlPoints)
		: BezierCurve(controlPoints)
	{
		m_tag = "Bezier C0";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device, L"../BuD/shaders/bezier_curve_c0_gs.hlsl", { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/bezier_with_polygon_ps.hlsl", { sizeof(Vector4) });

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this, device](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));

				struct GSResource
				{
					uint32_t samples;
					uint32_t drawPolygon;
				};

				auto rect = GetSurroundingRectangle(view, projection, device.BufferWidth(), device.BufferHeight());
				auto longSide = max(rect.right - rect.left, rect.bottom - rect.top);

				GSResource resource = { longSide / 10, m_drawPolygon };
				entity->GeometryShader()->UpdateConstantBuffer(0, &resource, sizeof(GSResource));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);

		OnUpdate();
	}

	void BezierCurveC0::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}

	void BezierCurveC0::OnUpdate()
	{
		int controlPointsCount = m_controlPoints.size();

		std::vector<Vector3> controlPoints;
		std::vector<unsigned short> controlPointsIndices;

		controlPoints.reserve(controlPointsCount);
		controlPointsIndices.reserve(2 * controlPointsCount);

		for (int i = 0; i < controlPointsCount; i++)
		{
			auto& point = m_controlPoints[i];
			controlPoints.push_back(point->Position());
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

		m_meshes[0]->VertexBuffer()->Update(controlPoints.data(), controlPoints.size() * sizeof(Vector3));
		m_meshes[0]->IndexBuffer()->Update(controlPointsIndices.data(), controlPointsIndices.size() * sizeof(unsigned short));
	}
}