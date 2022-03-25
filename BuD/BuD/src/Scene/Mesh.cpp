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
		std::function<void(std::shared_ptr<AbstractCamera>, Mesh*)> updateFunc
	)
		: m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer), m_pixelShader(pixelShader), m_vertexShader(vertexShader), m_updateFunc(updateFunc)
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

	void Mesh::UpdateConstantBuffers(std::shared_ptr<AbstractCamera> camera)
	{
		m_updateFunc(camera, this);
	}
}
