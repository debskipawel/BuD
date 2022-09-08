#include "Torus.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

#include <Objects/Scene.h>

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

	Torus::Torus(Scene& scene, const DX11Device& device, const Vector3& position, float largeRadius, float smallRadius)
		: SceneObject(scene), Parameterized2DObjectCPU(), m_largeRadius(largeRadius), m_smallRadius(smallRadius)
	{
		m_tag = "Torus";
		m_samplesU = m_samplesV = 8;

		Meshify();

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data());

		auto mesh = std::make_shared<Mesh>(vertexShader, pixelShader, vertexBuffer, indexBuffer,
			[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		m_meshes.push_back(mesh);
		MoveTo(position);
	}

	Vector3 Torus::GetPoint(Vector2 parameter)
	{
		float cu = cosf(parameter.x), su = sinf(parameter.x);
		float cv = cosf(parameter.y), sv = sinf(parameter.y);

		return Vector3
		{
			(m_largeRadius + m_smallRadius * cv) * cu,
			m_smallRadius * sv,
			(m_largeRadius + m_smallRadius * cv) * su
		};
	}

	void Torus::OnUpdate()
	{
		Meshify();

		// update renderable mesh with new vertices
		m_meshes[0]->VertexBuffer()->Update(m_vertices.data(), m_vertices.size() * sizeof(Vector3));
		m_meshes[0]->IndexBuffer()->Update(m_indices.data(), m_indices.size() * sizeof(unsigned short));
	}

	void Torus::OnSelect()
	{
		SceneObject::OnSelect();
		m_color = { 0.7f, 0.5f, 0.0f };
	}

	void Torus::OnUnselect()
	{
		SceneObject::OnUnselect();
		m_color = { 1.0f, 1.0f, 1.0f };
	}

	void Torus::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}
}