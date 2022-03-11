#pragma once

#include "Scene/RenderableSceneEntity.h"

namespace BuD
{
	class Renderer
	{
	public:
		virtual void Begin() = 0;
		virtual void Draw(const RenderableSceneEntity& entity) = 0;
		virtual void End() = 0;
		
		virtual void UpdateBufferSize(int width, int height) = 0;

	protected:
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}