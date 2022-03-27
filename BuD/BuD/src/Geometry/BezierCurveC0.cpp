#include "BezierCurveC0.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <imgui.h>
#include <algorithm>

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
		: m_color(1.0f, 1.0f, 1.0f), m_controlPoints(controlPoints)
	{
		m_tag = "Bezier C0";
		m_controlPoints.reserve(16);
		m_meshes.reserve(1);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device.Raw(), L"../BuD/shaders/pos_transf_vs.hlsl", elements);
		auto geometryShader = DX11ShaderLoader::Get()->GSLoad(device.Raw(), L"../BuD/shaders/bezier_curve_c0_gs.hlsl");
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/solid_color_ps.hlsl");

		vertexShader->AddConstantBuffer(VSConstantBuffer(device));
		pixelShader->AddConstantBuffer(PSConstantBuffer(device));

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, 16 * sizeof(Vector3), elements, nullptr);
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 16 * sizeof(unsigned short), nullptr, D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				std::copy_if(m_controlPoints.begin(), m_controlPoints.end(), m_controlPoints.begin(), [](SceneObject* obj) { return !obj->ShouldBeDeleted(); });

				std::vector<Vector3> controlPoints;
				std::vector<unsigned short> controlPointsIndices;
				controlPoints.reserve(m_controlPoints.size());
				controlPointsIndices.reserve((m_controlPoints.size() + 3) / 4 * 4);

				for (int i = 0; i < m_controlPoints.size(); i++)
				{
					auto& point = m_controlPoints[i];
					controlPoints.push_back(point->GetMesh(0)->m_position);
					controlPointsIndices.push_back(i);

					if ((i + 1) % 4 == 0)
					{
						controlPointsIndices.push_back(i);
					}
				}

				auto extraIndices = max(0, (static_cast<int>(m_controlPoints.size()) + 3) / 4 * 4 - static_cast<int>(controlPointsIndices.size()));

				for (int i = 0; i < extraIndices; i++)
				{
					controlPointsIndices.push_back(m_controlPoints.size() - 1);
				}

				entity->VertexBuffer()->Update(controlPoints.data(), controlPoints.size() * sizeof(Vector3));
				entity->IndexBuffer()->Update(controlPointsIndices.data(), controlPointsIndices.size() * sizeof(unsigned short));

				auto matrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		mesh->SetGS(geometryShader);

		m_meshes.push_back(mesh);
	}

	void BezierCurveC0::DrawGui()
	{
		ImGui::Text("Control points");
		for (auto& controlPoint : m_controlPoints)
		{
			std::string name = "Point " + std::to_string(controlPoint->Id());

			if (ImGui::TreeNode(name.c_str()))
			{
				controlPoint->DrawGui();

				auto removeName = "Remove CP " + std::to_string(controlPoint->Id());

				if (ImGui::Button(removeName.c_str()))
				{
					RemoveControlPoint(controlPoint);
				}

				ImGui::TreePop();
				ImGui::Separator();
			}
		}
	}

	void BezierCurveC0::AddControlPoint(SceneObject* obj)
	{
		m_controlPoints.push_back(obj);
	}

	void BezierCurveC0::RemoveControlPoint(SceneObject* obj)
	{
		auto res = std::find(m_controlPoints.begin(), m_controlPoints.end(), obj);
		
		if (res != m_controlPoints.end())
		{
			m_controlPoints.erase(res);
		}
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
