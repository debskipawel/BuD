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
		
		auto deBoorPixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_light_blue_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		geometryShader->AddConstantBuffer(GSConstantBuffer(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);
		auto polygonIndexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this, device](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				FilterControlPoints();
				UpdateCentroid();
				CalculateBernsteinPoints();

				size_t controlPointsCount = m_controlPoints.size();

				if (controlPointsCount < 4)
				{
					entity->VertexBuffer()->Update(nullptr, 0);
					entity->IndexBuffer()->Update(nullptr, 0);

					return;
				}

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

				entity->VertexBuffer()->Update(m_bernsteinPoints.data(), (m_bernsteinPoints.size()) * sizeof(Vector3));
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

		m_deBoorPolygonMesh = std::make_shared<Mesh>(vertexShader, deBoorPixelShader, vertexBuffer, polygonIndexBuffer,
			[this, device](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				size_t controlPointsCount = m_controlPoints.size();

				std::vector<Vector3> vertices(controlPointsCount);
				std::vector<unsigned short> indices(controlPointsCount);

				for (int i = 0; i < m_controlPoints.size(); i++)
				{
					indices[i] = i;
					vertices[i] = m_controlPoints[i]->GetMesh(0)->m_position;
				}

				entity->VertexBuffer()->Update(vertices.data(), vertices.size() * sizeof(Vector3));
				entity->IndexBuffer()->Update(indices.data(), indices.size() * sizeof(unsigned short));

				auto matrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);
	}

	void BezierCurveC2::UpdateBezierPoints(int modifiedIndex)
	{
		auto segment = modifiedIndex / 3;
		auto segmentPart = modifiedIndex % 3;

		switch (segmentPart)
		{
			case 0:  // segment end point
			{
				auto& bez = m_bernsteinPoints[modifiedIndex];
				auto& prev = m_controlPoints[segment]->GetMesh(0)->m_position;
				auto& next = m_controlPoints[segment + 2]->GetMesh(0)->m_position;

				auto newDeBoor = 1.5f * bez - 0.25f * (prev + next);
				m_controlPoints[segment + 1]->MoveTo(newDeBoor);

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

				break;
			}
		}

		CalculateBernsteinPoints();
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

		for (size_t i = 1; i < controlPointsCount - 2; i++)
		{
			auto& a = m_controlPoints[i - 1]->GetMesh(0)->m_position;
			auto& b = m_controlPoints[i]->GetMesh(0)->m_position;
			auto& c = m_controlPoints[i + 1]->GetMesh(0)->m_position;

			auto ba = (a - b) / 3.0f;
			auto bc = (c - b) / 3.0f;

			auto endPt = b + (ba + bc) / 2.0f;
			auto middleFirst = b + bc;
			auto middleSecond = middleFirst + bc;

			m_bernsteinPoints.push_back(endPt);
			m_bernsteinPoints.push_back(middleFirst);
			m_bernsteinPoints.push_back(middleSecond);
		}

		auto& a = m_controlPoints[controlPointsCount - 3]->GetMesh(0)->m_position;
		auto& b = m_controlPoints[controlPointsCount - 2]->GetMesh(0)->m_position;
		auto& c = m_controlPoints[controlPointsCount - 1]->GetMesh(0)->m_position;

		auto ba = (a - b) / 3.0f;
		auto bc = (c - b) / 3.0f;

		m_bernsteinPoints.push_back(b + (ba + bc) / 2.0f);
	}

	bool BezierCurveC2::DrawGui()
	{
		bool wasChanged = false;

		if (m_inBernstein)
		{
			wasChanged |= DrawGuiForEditingBezierPoints();
		}
		else
		{
			wasChanged |= DrawGuiForEditingControlPoints();
			ImGui::NewLine();
			wasChanged |= DrawGuiForAddingControlPoints();
			ImGui::NewLine();
		}

		ImGui::Checkbox("Bernstein base", &m_inBernstein);
		ImGui::NewLine();

		ImGui::Text("Draw Bezier polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##bp", &m_drawPolygon);

		bool deBoorCopy = m_drawDeBoorPolygon;
		ImGui::Text("Draw De Boor polygon:");
		ImGui::SameLine();
		ImGui::Checkbox("##dbp", &m_drawDeBoorPolygon);

		if (m_drawDeBoorPolygon ^ deBoorCopy)
		{
			if (m_drawDeBoorPolygon)
			{
				m_meshes.push_back(m_deBoorPolygonMesh);
			}
			else
			{
				m_meshes.erase(std::find(m_meshes.begin(), m_meshes.end(), m_deBoorPolygonMesh));
			}
		}

		return wasChanged;
	}

	bool BezierCurveC2::DrawGuiForEditingBezierPoints()
	{
		int i = 0;

		ImGui::Text("Bezier points");

		for (auto& point : m_bernsteinPoints)
		{
			std::string name = "Bezier point " + std::to_string(i++) + " ##" + std::to_string(i);

			if (ImGui::TreeNode(name.c_str()))
			{
				Vector3 pointCopy = point;

				ImGui::Text("Position");
				ImGui::DragFloat("x", &point.x, 0.1f);
				ImGui::DragFloat("y", &point.y, 0.1f);
				ImGui::DragFloat("z", &point.z, 0.1f);

				if ((pointCopy - point).LengthSquared())
				{
					UpdateBezierPoints(i - 1);
				}

				ImGui::TreePop();
			}

			ImGui::Separator();
		}

		return true;
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

	void BezierCurveC2::RemoveControlPoint(int index)
	{
		BezierCurve::RemoveControlPoint(index);
		CalculateBernsteinPoints();
	}

	std::vector<Vector3> BezierCurveC2::VirtualControlPoints()
	{
		return m_inBernstein ? m_bernsteinPoints : std::vector<Vector3>();
	}

	GeometryType BezierCurveC2::GetType()
	{
		return GeometryType::BEZIER_C2;
	}

	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_vsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_gsConstantBuffer = nullptr;
	std::shared_ptr<DX11ConstantBuffer> BezierCurveC2::s_psConstantBuffer = nullptr;
}
