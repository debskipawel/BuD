#pragma once

#include "DirectX11/Buffers/DX11ConstantBuffer.h"
#include "DirectX11/Buffers/DX11VertexBuffer.h"
#include "DirectX11/Buffers/DX11IndexBuffer.h"

#include "Mesh.h"

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class Cursor
	{
	public:
		Cursor(Vector3 position, const DX11Device& device);
		~Cursor() = default;

		virtual void DrawGui();

		static std::shared_ptr<Cursor> GetCursorAt(Vector3 position, const DX11Device& device);

		inline std::shared_ptr<Mesh> GetModel() { return m_model; }

	private:
		std::shared_ptr<Mesh> m_model;

		static std::shared_ptr<Cursor> s_cursor;
	};
}
