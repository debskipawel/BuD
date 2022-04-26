#pragma once

#include "SceneObject.h"

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class Point : public SceneObject
	{
	public:
		Point(Vector3 position, const DX11Device& device);
		~Point() = default;

		virtual bool DrawGui() override;

		virtual GeometryType GetType() override { return GeometryType::POINT; }

		virtual void Select() override 
		{ 
			SceneObject::Select();
			m_color = { 0.7f, 0.5f, 0.0f }; 
		}
		
		virtual void Unselect() override
		{ 
			SceneObject::Unselect();
			m_color = { 1.0f, 1.0f, 1.0f }; 
		}

		inline virtual void RotateTo(const Vector3& rotation) override { }
		inline virtual void RotateTo(const Quaternion& rotation) override { }
		inline virtual void ScaleTo(const Vector3& scale) override { }

		static std::shared_ptr<Mesh> GetMesh(const DX11Device& device);

	private:

		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		static std::shared_ptr<DX11VertexBuffer> GetVB(const DX11Device& device);
		static std::shared_ptr<DX11IndexBuffer> GetIB(const DX11Device& device);

		static std::shared_ptr<DX11VertexBuffer> s_vertexBuffer;
		static std::shared_ptr<DX11IndexBuffer> s_indexBuffer;
	};
}
