#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class IEditable
	{
	public:
		virtual void DrawGui() = 0;

	protected:
		virtual void EditorFor(Vector2& vec, const char* title, float min = 0.0f, float max = 0.0f);
		virtual void EditorFor(Vector2& vec, float min = 0.0f, float max = 0.0f);
		virtual void EditorFor(Vector3& vec, const char* title, float min = 0.0f, float max = 0.0f);
		virtual void EditorFor(Vector3& vec, float min = 0.0f, float max = 0.0f);
	};
}
