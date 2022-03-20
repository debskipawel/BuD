#pragma once

#include <SimpleMath.h>

#include "Geometry/SceneObject.h"

using namespace DirectX::SimpleMath;

namespace BuD
{
	class ObjectsEditor
	{
	public:
		ObjectsEditor(std::vector<std::shared_ptr<SceneObject>>& objects);

		Vector3 CursorPosition() const { return m_cursorPosition; }

		void DrawGui(const DX11Device& device);

	protected:
		SceneObject* m_selectedObject = nullptr;
		std::vector<std::shared_ptr<SceneObject>>& m_objects;
		Vector3 m_cursorPosition;
	};
}
