#pragma once

#include "Scene/RenderableSceneEntity.h"

namespace BuD
{
	class Renderer
	{
	public:
		virtual void Begin() = 0;
		virtual void Draw(std::shared_ptr<RenderableSceneEntity> entity, std::shared_ptr<AbstractCamera> camera) = 0;
		virtual void End() = 0;
		
		virtual void UpdateBuffersSize(int width, int height, int topX, int topY) = 0;

	protected:
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}