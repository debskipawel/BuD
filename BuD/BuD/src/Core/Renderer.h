#pragma once

#include "Scene/Mesh.h"

namespace BuD
{
	class Renderer
	{
	public:
		virtual void Begin() = 0;
		virtual void Draw(std::shared_ptr<Mesh> entity, std::shared_ptr<AbstractCamera> camera, uint32_t id = 0) = 0;
		virtual void End() = 0;
		
		virtual void UpdateBuffersSize(int width, int height) = 0;

	protected:
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}