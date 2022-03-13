#pragma once

#include <BuD.h>

class Cube
{
public:
	static std::shared_ptr<BuD::RenderableSceneEntity> GetEntity(const BuD::DX11Device& device);

private:
	static std::shared_ptr<BuD::DX11VertexBuffer> GetVB(const BuD::DX11Device& device);
	static std::shared_ptr<BuD::DX11IndexBuffer> GetIB(const BuD::DX11Device& device);

	static std::shared_ptr<BuD::DX11VertexBuffer> s_vertexBuffer;
	static std::shared_ptr<BuD::DX11IndexBuffer> s_indexBuffer;
};