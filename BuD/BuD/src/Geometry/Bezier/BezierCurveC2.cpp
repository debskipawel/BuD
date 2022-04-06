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
		
		CalculateBernsteinPoints();

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

				size_t controlPointsCount = m_controlPoints.size();

				if (controlPointsCount < 4)
				{
					entity->VertexBuffer()->Update(nullptr, 0);
					entity->IndexBuffer()->Update(nullptr, 0);

					return;
				}

				std::vector<unsigned short> indices;
				indices.reserve((controlPointsCount - 3) * 4);

				for (int i = 3; i < m_bernsteinPoints.size() - 2; i++)
				{
					indices.push_back(i - 3);

					if (indices.size() % 4 == 0)
					{
						indices.push_back(i - 3);
					}
				}

				entity->VertexBuffer()->Update(m_bernsteinPoints.data() + 3, (m_bernsteinPoints.size() - 5) * sizeof(Vector3));
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

	void BezierCurveC2::CalculateBernsteinPoints()
	{
		m_bernsteinPoints.clear();
		int controlPointsCount = m_controlPoints.size();

		if (controlPointsCount < 4)
		{
			return;
		}

		m_bernsteinPoints.reserve(3 * controlPointsCount);

		for (size_t i = 0; i < controlPointsCount - 1; i++)
		{
			auto& a = m_controlPoints[i]->GetMesh(0)->m_position;
			auto& b = m_controlPoints[i + 1]->GetMesh(0)->m_position;

			m_bernsteinPoints.push_back(a);
			m_bernsteinPoints.push_back((2.0f * a + b) / 3.0f);
			m_bernsteinPoints.push_back((a + 2.0f * b) / 3.0f);
		}

		for (size_t i = 1; i < controlPointsCount - 1; i++)
		{
			m_bernsteinPoints[3 * i] = (m_bernsteinPoints[3 * i - 1] + m_bernsteinPoints[3 * i + 1]) / 2.0f;
		}
	}

	bool BezierCurveC2::DrawGui()
	{
		bool wasChanged = false;

		if (DrawGuiForEditingControlPoints())
		{
			wasChanged = true;
			CalculateBernsteinPoints();
		}

		ImGui::NewLine();

		wasChanged |= DrawGuiForAddingControlPoints();
		ImGui::NewLine();

		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &m_drawPolygon);

		return wasChanged;
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

	void BezierCurveC2::AddControlPoint(SceneObject* obj)
	{
		BezierCurve::AddControlPoint(obj);
		CalculateBernsteinPoints();
	}

	void BezierCurveC2::RemoveControlPoint(SceneObject* obj)
	{
		BezierCurve::RemoveControlPoint(obj);
		CalculateBernsteinPoints();
	}

	GeometryType BezierCurveC2::GetType()
	{
		return GeometryType::BEZIER_C2;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_gsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_psConstantBuffer = nullptr;
}
