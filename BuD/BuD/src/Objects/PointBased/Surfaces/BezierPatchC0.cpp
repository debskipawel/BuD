#include "BezierPatchC0.h"
#include "BezierSurfaceC0.h"

#include <algorithm>

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

	BezierPatchC0::BezierPatchC0(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC0* owner)
		: PointBasedObject(scene, controlPoints), m_owner(owner), m_showBezierPolygon(false)
	{
		m_samplesU = samplesU;
		m_samplesV = samplesV;

		Meshify();

		m_tag = "Patch C0";

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });

		auto vertexBuffer = std::make_shared<DX11VertexBuffer>(device, m_vertices.size() * sizeof(Vector3), elements, m_vertices.data());
		auto indexBuffer = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, m_indices.size() * sizeof(unsigned short), m_indices.data());

		auto polygonVB = std::make_shared<DX11VertexBuffer>(device, m_controlPoints.size() * sizeof(Vector3), elements);
		auto polygonIB = std::make_shared<DX11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 2 * m_controlPoints.size() * sizeof(unsigned short), nullptr, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		auto mesh = std::make_shared<Mesh>(vertexShader, nullptr, pixelShader, vertexBuffer, indexBuffer,
			[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		m_bezierPolygonMesh = std::make_shared<Mesh>(vertexShader, nullptr, pixelShader, polygonVB, polygonIB,
			[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * view * projection;
				Vector3 polygonColor = { 1.0f, 0.0f, 0.0f };

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &polygonColor, sizeof(Vector3));
			}
		);

		m_meshes.push_back(mesh);

		OnUpdate();
	}

	void BezierPatchC0::ToggleBezierPolygon(bool show)
	{
		if (show)
		{
			if (m_showBezierPolygon)
			{
				return;
			}

			m_showBezierPolygon = true;
			m_meshes.push_back(m_bezierPolygonMesh);
		}
		else
		{
			m_showBezierPolygon = false;
			m_meshes.erase(std::find(m_meshes.begin(), m_meshes.end(), m_bezierPolygonMesh));
		}
	}
	
	void BezierPatchC0::OnSelect()
	{
		SceneObject::OnSelect();
		m_color = { 0.7f, 0.5f, 0.0f };
	}

	void BezierPatchC0::OnUnselect()
	{
		SceneObject::OnUnselect();
		m_color = { 1.0f, 1.0f, 1.0f };
	}

	Vector3 BezierPatchC0::GetPoint(Vector2 param)
	{
		if (param.x != m_prevU)
		{
			m_prevU = param.x;
			m_uDeCasteljau = DeCasteljau(param.x);
		}

		auto vDeCasteljau = DeCasteljau(param.y);

		Vector3 result = {};

		for (int u = 0; u < 4; u++)
		{
			Vector3 partResult = {};

			for (int v = 0; v < 4; v++)
			{
				auto point = m_controlPoints[u * 4 + v]->Position();
				partResult += point * vDeCasteljau[v];
			}

			result += partResult * m_uDeCasteljau[u];
		}

		return result;
	}
	
	std::array<float, 4> BezierPatchC0::DeCasteljau(float t)
	{
		auto res = std::array<float, 4> { 0.0f };
		res[0] = 1.0f;

 		float u = 1.0f - t;

		for (int j = 1; j <= 3; j++)
		{
			for (int i = j; i >= 1; i--)
			{
				res[i] = res[i] * u + res[i - 1] * t;
			}

			res[0] = res[0] * u;
		}

		return res;
	}

	void BezierPatchC0::OnUpdate()
	{
		if (m_controlPoints.size() != 16)
		{
			m_scene.RemoveSceneObject(m_id);
			return;
		}

		Meshify();

		m_meshes[0]->VertexBuffer()->Update(m_vertices.data(), m_vertices.size() * sizeof(Vector3));
		m_meshes[0]->IndexBuffer()->Update(m_indices.data(), m_indices.size() * sizeof(unsigned short));

		std::vector<Vector3> polygonVertices(16);
		std::vector<unsigned short> polygonIndices;

		std::transform(m_controlPoints.begin(), m_controlPoints.end(), polygonVertices.begin(), [](SceneObject* obj) { return obj->Position(); });

		for (int i = 0; i < 16; i++)
		{
			if ((i + 1) % 4 == i % 4 + 1)
			{
				polygonIndices.push_back(i);
				polygonIndices.push_back(i + 1);
			}

			if (i + 4 < 16)
			{
				polygonIndices.push_back(i);
				polygonIndices.push_back(i + 4);
			}
		}

		m_bezierPolygonMesh->VertexBuffer()->Update(polygonVertices.data(), polygonVertices.size() * sizeof(Vector3));
		m_bezierPolygonMesh->IndexBuffer()->Update(polygonIndices.data(), polygonIndices.size() * sizeof(unsigned short));
	}

	void BezierPatchC0::OnDelete()
	{
		PointBasedObject::OnDelete();

		if (m_owner)
		{
			m_owner->OnDeletePatch(this);
		}
	}
	
	void BezierPatchC0::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}
}
