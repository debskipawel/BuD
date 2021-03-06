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
			std::shared_ptr<DX11GeometryShader> geometryShader,
			std::shared_ptr<DX11PixelShader>  pixelShader,
			std::shared_ptr<DX11VertexBuffer> vertexBuffer,
			std::shared_ptr<DX11IndexBuffer>  indexBuffer,
			std::function<void(const dxm::Matrix& view, const dxm::Matrix& projection, Mesh*)> onStartRendering = [](const dxm::Matrix&, const dxm::Matrix&, Mesh*) {},
			std::function<bool(int, Mesh*)> onRunUpdate = [](int, Mesh*) { return true; }
		);

		inline std::shared_ptr<DX11VertexShader> VertexShader() const { return m_vertexShader; }
		inline std::shared_ptr<DX11GeometryShader> GeometryShader() const { return m_geometryShader; }
		inline std::shared_ptr<DX11PixelShader> PixelShader() const { return m_pixelShader; }

		inline std::shared_ptr<DX11VertexBuffer> VertexBuffer() const { return m_vertexBuffer; }
		inline std::shared_ptr<DX11IndexBuffer> IndexBuffer() const { return m_indexBuffer; }

		inline virtual bool Finished() { return m_finished; }

		virtual dxm::Matrix GetModelMatrix();
		virtual void UpdateRotation();

		dxm::Vector3 m_position = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_rotation = { 0.0f, 0.0f, 0.0f };
		dxm::Vector3 m_scale = { 1.0f, 1.0f, 1.0f };
		dxm::Quaternion m_quatRotation;

	protected:
		Mesh() = default;

		void OnStartRendering(const dxm::Matrix& view, const dxm::Matrix& projection);
		void OnRunUpdate();

		std::shared_ptr<DX11VertexShader> m_vertexShader = nullptr;
		std::shared_ptr<DX11GeometryShader> m_geometryShader = nullptr;
		std::shared_ptr<DX11PixelShader> m_pixelShader = nullptr;

		std::shared_ptr<DX11VertexBuffer> m_vertexBuffer;
		std::shared_ptr<DX11IndexBuffer> m_indexBuffer;

		std::function<void(const dxm::Matrix&, const dxm::Matrix&, Mesh*)> m_onStartRendering;
		std::function<bool(int, Mesh*)> m_onRunUpdate;

		int m_shaderRun;
		bool m_finished;

		friend class DX11Renderer;
		friend class DX11StereoscopicRenderer;
	};
}
