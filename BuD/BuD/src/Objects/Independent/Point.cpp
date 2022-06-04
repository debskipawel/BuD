#include "Point.h"

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

#include <Objects/Scene.h>

namespace BuD
{
	static std::vector<Vector3> vertices =
	{
		{ -0.03f, -0.03f, -0.03f },
		{ +0.03f, -0.03f, -0.03f },
		{ +0.03f, +0.03f, -0.03f },
		{ -0.03f, +0.03f, -0.03f },

		{ -0.03f, -0.03f, +0.03f },
		{ +0.03f, +0.03f, +0.03f },
		{ +0.03f, -0.03f, +0.03f },
		{ -0.03f, +0.03f, +0.03f },

		{ -0.03f, -0.03f, -0.03f },
		{ -0.03f, +0.03f, -0.03f },
		{ -0.03f, -0.03f, +0.03f },
		{ -0.03f, +0.03f, +0.03f },

		{ +0.03f, -0.03f, -0.03f },
		{ +0.03f, +0.03f, -0.03f },
		{ +0.03f, -0.03f, +0.03f },
		{ +0.03f, +0.03f, +0.03f },

		{ +0.03f, +0.03f, -0.03f },
		{ +0.03f, +0.03f, +0.03f },
		{ -0.03f, +0.03f, -0.03f },
		{ -0.03f, +0.03f, +0.03f },

		{ +0.03f, -0.03f, -0.03f },
		{ +0.03f, -0.03f, +0.03f },
		{ -0.03f, -0.03f, -0.03f },
		{ -0.03f, -0.03f, +0.03f },
	};

	static std::vector<unsigned short> indices =
	{
		0,2,1, 0,3,2,
		4,6,5, 4,6,7,
		8,10,11, 8,9,11,
		12,14,15, 12,13,15,
		16,18,19, 16,17,19,
		20,22,23, 20,21,23
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	Point::Point(Scene& scene, const DX11Device& device, const Vector3& position)
		: SceneObject(scene)
	{
		m_tag = "Point";
		m_meshes.reserve(1);

		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });

		auto mesh = std::make_shared<Mesh>(vertexShader, nullptr, pixelShader, GetVB(device), GetIB(device),
			[this](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				auto matrix = entity->GetModelMatrix() * view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &m_color, sizeof(Vector3));
			}
		);

		mesh->m_position = position;

		m_meshes.push_back(mesh);
	}

	void Point::Accept(AbstractVisitor& visitor)
	{
		visitor.Action(*this);
	}

	void Point::OnUpdate()
	{
		for (auto& dependent : m_dependentObjects)
		{
			dependent->OnUpdate();
		}
	}
	
	void Point::OnDelete()
	{
		for (auto& dependent : m_dependentObjects)
		{
			auto& controlPoints = dependent->m_controlPoints;

			while (true)
			{
				auto position = std::find(controlPoints.begin(), controlPoints.end(), this);

				if (position == controlPoints.end())
				{
					break;
				}

				controlPoints.erase(position);
			}

			dependent->OnUpdate();
		}
	}

	void Point::OnSelect()
	{
		SceneObject::OnSelect();
		m_color = { 0.7f, 0.5f, 0.0f };
	}

	void Point::OnUnselect()
	{
		SceneObject::OnUnselect();
		m_color = { 1.0f, 1.0f, 1.0f };
	}

	void Point::AddDependentObject(PointBasedObject* obj)
	{
		m_dependentObjects.insert(obj);
	}

	void Point::RemoveDependentObject(PointBasedObject* obj)
	{
		m_dependentObjects.erase(obj);
	}

	std::shared_ptr<Mesh> Point::GetMesh(const DX11Device& device)
	{
		auto vertexShader = DX11ShaderLoader::Get()->VSLoad(device, L"../BuD/shaders/pos_transf_vs.hlsl", elements, { sizeof(Matrix) });
		auto pixelShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/solid_color_ps.hlsl", { sizeof(Vector4) });

		auto mesh = std::make_shared<Mesh>(vertexShader, nullptr, pixelShader, GetVB(device), GetIB(device),
			[](const dxm::Matrix& view, const dxm::Matrix& projection, Mesh* entity)
			{
				Vector3 color = { 1.0f, 1.0f, 1.0f };
				auto matrix = entity->GetModelMatrix() * view * projection;

				entity->VertexShader()->UpdateConstantBuffer(0, &matrix, sizeof(Matrix));
				entity->PixelShader()->UpdateConstantBuffer(0, &color, sizeof(Vector3));
			}
		);

		return mesh;
	}

	std::shared_ptr<Point> Point::Merge(const DX11Device& device, Point* first, Point* second)
	{
		auto& scene = first->m_scene;
		
		if (scene.GetSceneObject(second->Id()).get() != second)
		{
			// two points are from different scenes, merging them makes no sense
			return nullptr;
		}

		auto newPoint = scene.CreatePoint(device, 0.5f * (first->Position() + second->Position()));

		for (auto& obj : first->m_dependentObjects)
		{
			obj->ReplaceControlPoint(first, newPoint.get());
			newPoint->AddDependentObject(obj);
		}

		for (auto& obj : second->m_dependentObjects)
		{
			obj->ReplaceControlPoint(second, newPoint.get());
			newPoint->AddDependentObject(obj);
		}

		scene.RemoveSceneObject(first->Id());
		scene.RemoveSceneObject(second->Id());

		return newPoint;
	}

	std::shared_ptr<BuD::DX11VertexBuffer> Point::GetVB(const BuD::DX11Device& device)
	{
		if (!s_vertexBuffer)
		{
			s_vertexBuffer = std::make_shared<BuD::DX11VertexBuffer>(
				device,
				vertices.size() * sizeof(Vector3),
				elements,
				vertices.data()
				);
		}

		return s_vertexBuffer;
	}

	std::shared_ptr<BuD::DX11IndexBuffer> Point::GetIB(const BuD::DX11Device& device)
	{
		if (!s_indexBuffer)
		{
			s_indexBuffer = std::make_shared<BuD::DX11IndexBuffer>(
				device,
				DXGI_FORMAT_R16_UINT,
				indices.size() * sizeof(unsigned short),
				indices.data()
				);
		}

		return s_indexBuffer;
	}

	std::shared_ptr<BuD::DX11VertexBuffer> Point::s_vertexBuffer = nullptr;
	std::shared_ptr<BuD::DX11IndexBuffer> Point::s_indexBuffer = nullptr;
}