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

		virtual void DrawGui() override;

		inline std::shared_ptr<Mesh> GetModel() { return m_model; }

	private:

		static std::shared_ptr<BuD::DX11VertexBuffer> GetVB(const BuD::DX11Device& device);
		static std::shared_ptr<BuD::DX11IndexBuffer> GetIB(const BuD::DX11Device& device);

		static std::shared_ptr<BuD::DX11VertexBuffer> s_vertexBuffer;
		static std::shared_ptr<BuD::DX11IndexBuffer> s_indexBuffer;

		static std::shared_ptr<DX11ConstantBuffer> ConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_constantBuffer;
	};
}
