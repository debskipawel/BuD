#include "Torus.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <imgui.h>

namespace BuD
{
	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	Torus::Torus(const DX11Device& device, float largeRadius, float smallRadius)
		: Parameterized2DEntity({ 0.0f, 0.0f }, { 6.28318f, 6.28318f }, TorusEquation()),
		m_largeRadius(largeRadius), m_smallRadius(smallRadius), m_color(1.0f, 1.0f, 1.0f)
	{
		m_tag = "Torus";
		m_meshes.reserve(1);

		UpdateSampleIntervals(8, 8);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data());

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this](std::shared_ptr<AbstractCamera> camera, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * camera->GetViewMatrix() * camera->GetProjectionMatrix();

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		m_meshes.push_back(mesh);
	}

	bool Torus::UpdateRadius(float largeRadius, float smallRadius)
	{
		if (m_largeRadius == largeRadius && m_smallRadius == smallRadius)
		{
			return false;
		}

		m_largeRadius = largeRadius;
		m_smallRadius = smallRadius;

		Sample();
		return true;
	}

	bool Torus::DrawGui()
	{
		bool wasChanged = false;

		Vector3 currPos = m_meshes[0]->m_position;
		Vector3 currPosCopy = currPos;

		ImGui::Text("Translation");
		ImGui::DragFloat("t(x)", &currPos.x, 0.5f);
		ImGui::DragFloat("t(y)", &currPos.y, 0.5f);
		ImGui::DragFloat("t(z)", &currPos.z, 0.5f);
		ImGui::NewLine();

		if ((currPos - currPosCopy).LengthSquared())
		{
			this->MoveTo(currPos);
			wasChanged = true;
		}

		Vector3 currRot = m_meshes[0]->m_rotation;
		Vector3 currRotCopy = currRot;

		ImGui::Text("Rotation");
		ImGui::DragFloat("r(x)", &currRot.x, 1.0f);
		ImGui::DragFloat("r(y)", &currRot.y, 1.0f);
		ImGui::DragFloat("r(z)", &currRot.z, 1.0f);
		
		if ((currRot - currRotCopy).LengthSquared())
		{
			this->RotateTo(currRot);
			wasChanged = true;
		}
		
		ImGui::NewLine();

		Vector3 currScale = m_meshes[0]->m_scale;
		Vector3 currScaleCopy = currRot;
		
		ImGui::Text("Scale");
		ImGui::DragFloat("s(x)", &currScale.x, 0.1f);
		ImGui::DragFloat("s(y)", &currScale.y, 0.1f);
		ImGui::DragFloat("s(z)", &currScale.z, 0.1f);
		ImGui::NewLine();
		
		if ((currScale - currScaleCopy).LengthSquared())
		{
			this->ScaleTo(currScale);
			wasChanged = true;
		}

		Parameterized2DEntity::DrawGui();

		float r = m_smallRadius;
		float R = m_largeRadius;

		const float minSmallRadius = 0.001f;
		const float minLargeRadius = 0.001f;
		const float maxSmallRadius = m_largeRadius;

		ImGui::Text("Torus radius");
		ImGui::DragFloat("Large", &R, 0.1f, 0.001f);
		ImGui::DragFloat("Small", &r, 0.1f, 0.001f, R);

		R = R < minLargeRadius ? minLargeRadius : R;
		r = r < minSmallRadius ? minSmallRadius : r;
		r = r > maxSmallRadius ? maxSmallRadius : r;

		if (UpdateRadius(R, r))
		{
			UpdateRenderableModel();
			wasChanged = true;
		}

		return wasChanged;
	}

	void Torus::UpdateRenderableModel()
	{
		m_meshes[0]->VertexBuffer()->Update(m_vertices.data(), m_vertices.size() * sizeof(Vector3));
		m_meshes[0]->IndexBuffer()->Update(m_indices.data(), m_indices.size() * sizeof(unsigned short));
	}
}
