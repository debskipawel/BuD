#include "Mesh.h"

#include <SimpleMath.h>

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	Mesh::Mesh(
		std::shared_ptr<DX11VertexShader> vertexShader,
		std::shared_ptr<DX11PixelShader> pixelShader,
		std::shared_ptr<DX11VertexBuffer> vertexBuffer,
		std::shared_ptr<DX11IndexBuffer> indexBuffer,
		std::function<void(const dxm::Matrix&, const dxm::Matrix&, Mesh*)> onStartRendering,
		std::function<bool(int, Mesh*)> onRunUpdate
	)
		: m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer),
		m_vertexShader(vertexShader), m_pixelShader(pixelShader),
		m_onStartRendering(onStartRendering), m_onRunUpdate(onRunUpdate),
		m_finished(false), m_shaderRun(0)
	{
	}

	dxm::Matrix Mesh::GetModelMatrix()
	{
		auto rot = dxm::Matrix::CreateFromQuaternion(m_quatRotation);
		auto trans = dxm::Matrix::CreateTranslation(m_position);
		auto scale = dxm::Matrix::CreateScale(m_scale);

		return scale * rot * trans;
	}

	void Mesh::UpdateRotation()
	{
		auto rotate = dxm::Matrix::CreateRotationX(dx::XMConvertToRadians(m_rotation.x))
			* dxm::Matrix::CreateRotationY(dx::XMConvertToRadians(m_rotation.y))
			* dxm::Matrix::CreateRotationZ(dx::XMConvertToRadians(m_rotation.z));
		m_quatRotation = dxm::Quaternion::CreateFromRotationMatrix(rotate);
	}

	void Mesh::OnStartRendering(const dxm::Matrix& view, const dxm::Matrix& projection)
	{
		m_shaderRun = 0;
		m_finished = false;

		m_onStartRendering(view, projection, this);
	}

	void Mesh::OnRunUpdate()
	{
		m_finished = m_onRunUpdate(m_shaderRun, this);
		m_shaderRun++;
	}
}
