#pragma once

namespace BuD
{
	class Renderer
	{
	public:
		virtual void Draw() = 0;
		virtual void UpdateBufferSize(int width, int height) = 0;

	protected:
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}