#include "BezierCurveC2.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

#include <Objects/Scene.h>
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

	BezierCurveC2::BezierCurveC2(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints)
		: BezierCurve(scene, controlPoints)
	{
		m_tag = "Bezier C2";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device, L"../BuD/shaders/bezier_curve_c0_gs.hlsl", { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/bezier_with_polygon_ps.hlsl", { sizeof(Vector4) });

		auto deBoorPixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_light_blue_ps.hlsl", {});

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);
		auto polygonVertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto polygonIndexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		auto mesh = std::make_shared<Mesh>(vertexShader, geometryShader, pixelShader, vertexBuffer, indexBuffer,
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

		m_meshes.push_back(mesh);

		m_deBoorPolygonMesh = std::make_shared<Mesh>(vertexShader, nullptr, deBoorPixelShader, polygonVertexBuffer, polygonIndexBuffer,
			[this, device](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
			}
		);

		OnUpdate();
	}

	void BezierCurveC2::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}

	void BezierCurveC2::OnUpdate()
	{
		size_t controlPointsCount = m_controlPoints.size();

		// stop condition
		if (controlPointsCount < 4)
		{
			m_meshes[0]->VertexBuffer()->Update(nullptr, 0);
			m_meshes[0]->IndexBuffer()->Update(nullptr, 0);

			return;
		}

		// transforming nodes from B-Spline basis to Bernstein basis
		m_bernsteinPoints.clear();
		m_bernsteinPoints.reserve(3 * controlPointsCount);

		for (size_t i = 1; i < controlPointsCount - 2; i++)
		{
			auto a = m_controlPoints[i - 1]->Position();
			auto b = m_controlPoints[i]->Position();
			auto c = m_controlPoints[i + 1]->Position();

			auto ba = (a - b) / 3.0f;
			auto bc = (c - b) / 3.0f;

			auto endPt = b + (ba + bc) / 2.0f;
			auto middleFirst = b + bc;
			auto middleSecond = middleFirst + bc;

			m_bernsteinPoints.push_back(endPt);
			m_bernsteinPoints.push_back(middleFirst);
			m_bernsteinPoints.push_back(middleSecond);
		}

		auto a = m_controlPoints[controlPointsCount - 3]->Position();
		auto b = m_controlPoints[controlPointsCount - 2]->Position();
		auto c = m_controlPoints[controlPointsCount - 1]->Position();

		auto ba = (a - b) / 3.0f;
		auto bc = (c - b) / 3.0f;

		m_bernsteinPoints.push_back(b + (ba + bc) / 2.0f);

		// calculating indices
		std::vector<unsigned short> indices;
		indices.reserve((controlPointsCount - 3) * 4);

		for (int i = 0; i < m_bernsteinPoints.size(); i++)
		{
			indices.push_back(i);

			if (indices.size() % 4 == 0)
			{
				indices.push_back(i);
			}
		}

		m_meshes[0]->VertexBuffer()->Update(m_bernsteinPoints.data(), m_bernsteinPoints.size() * sizeof(Vector3));
		m_meshes[0]->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));

		// indices for polygon mesh
		std::vector<Vector3> polygonVertices(controlPointsCount);
		std::vector<unsigned short> polygonIndices(controlPointsCount);

		for (int i = 0; i < m_controlPoints.size(); i++)
		{
			polygonIndices[i] = i;
			polygonVertices[i] = m_controlPoints[i]->Position();
		}

		m_deBoorPolygonMesh->VertexBuffer()->Update(polygonVertices.data(), polygonVertices.size() * sizeof(Vector3));
		m_deBoorPolygonMesh->IndexBuffer()->Update(polygonIndices.data(), polygonIndices.size() * sizeof(unsigned short));
	}

	void BezierCurveC2::ModifyBernsteinPoints(int modifiedIndex)
	{
		auto segment = modifiedIndex / 3;
		auto segmentPart = modifiedIndex % 3;

		switch (segmentPart)
		{
			case 0:  // segment end point
			{
				auto& bez = m_bernsteinPoints[modifiedIndex];
				auto prev = m_controlPoints[segment]->Position();
				auto next = m_controlPoints[segment + 2]->Position();

				auto newDeBoor = 1.5f * bez - 0.25f * (prev + next);
				m_controlPoints[segment + 1]->MoveTo(newDeBoor, true);

				break;
			}
			case 1:  // segment middle
			case 2:
			{
				auto& midFirst = m_bernsteinPoints[3 * segment + 1];
				auto& midSecond = m_bernsteinPoints[3 * segment + 2];

				auto vec = midSecond - midFirst;

				m_controlPoints[segment + 1]->MoveTo(midFirst - vec);
				m_controlPoints[segment + 2]->MoveTo(midSecond + vec);

				m_controlPoints[segment + 1]->OnUpdate();
				m_controlPoints[segment + 2]->OnUpdate();

				break;
			}
		}
	}
}