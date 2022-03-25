#pragma once

#include "Camera/AbstractCamera.h"

#include "DirectX11/Shaders/DX11VertexShader.h"
#include "DirectX11/Shaders/DX11GeometryShader.h"
#include "DirectX11/Shaders/DX11PixelShader.h"

#include "DirectX11/Buffers/DX11VertexBuffer.h"
#include "DirectX11/Buffers/DX11IndexBuffer.h"

#include <functional>

namespace BuD
{
	class Mesh
	{
	public:
		Mesh(
			std::shared_ptr<DX11VertexShader> vertexShader,
			std::shared_ptr<DX11PixelShader>  pixelShader,
			std::shared_ptr<DX11VertexBuffer> vertexBuffer,
			std::shared_ptr<DX11IndexBuffer>  indexBuffer,
			std::function<void(std::shared_ptr<AbstractCamera>, Mesh*)> updateFunc = [](std::shared_ptr<AbstractCamera>, Mesh*) {}
		);

		inline void SetGS(std::shared_ptr<DX11GeometryShader> shader) { m_geometryShader = shader; }

		inline std::shared_ptr<DX11VertexShader> VertexShader() const { return m_vertexShader; }
		inline std::shared_ptr<DX11PixelShader> PixelShader() const { return m_pixelShader; }
		
		inline std::shared_ptr<DX11VertexBuffer> VertexBuffer() const { return m_vertexBuffer; }
		inline std::shared_ptr<DX11IndexBuffer> IndexBuffer() const { return m_indexBuffer; }

		virtual dxm::Matrix GetModelMatrix();
		virtual void UpdateRotation();

		dxm::Vector3 m_position = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_rotation = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_scale = { 1.0f, 1.0f, 1.0f };
		dxm::Quaternion m_quatRotation;

	protected:
		Mesh() = default;

		void UpdateConstantBuffers(std::shared_ptr<AbstractCamera> camera);

		std::shared_ptr<DX11VertexShader> m_vertexShader = nullptr;
		std::shared_ptr<DX11GeometryShader> m_geometryShader = nullptr;
		std::shared_ptr<DX11PixelShader> m_pixelShader = nullptr;
		
		std::shared_ptr<DX11VertexBuffer> m_vertexBuffer;
		std::shared_ptr<DX11IndexBuffer> m_indexBuffer;

		std::function<void(std::shared_ptr<AbstractCamera>, Mesh*)> m_updateFunc;

		friend class DX11Renderer;
	};
}
