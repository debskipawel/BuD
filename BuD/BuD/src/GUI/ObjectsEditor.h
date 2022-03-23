#pragma once

#include <SimpleMath.h>

#include "Geometry/SceneObject.h"

using namespace DirectX::SimpleMath;

namespace BuD
{
	class ObjectsEditor
	{
	public:
		ObjectsEditor(std::vector<std::shared_ptr<SceneObject>>& objects, std::shared_ptr<AbstractCamera> camera, std::shared_ptr<Win32Window> window);

		Vector3 CursorPosition() const { return m_cursorPosition; }
		void DrawGui(const DX11Device& device);
		void SetCursorTo(int pixelX, int pixelY);
		void SelectionChanged();

	protected:
		void DrawMainSettings(const DX11Device& device);
		void DrawSelectableList(const DX11Device& device);
		void DrawSelectableConfig(const DX11Device& device);

		std::shared_ptr<AbstractCamera> m_camera;
		std::shared_ptr<Win32Window> m_window;

		SceneObject* m_selectedObject = nullptr;
		std::vector<std::shared_ptr<SceneObject>>& m_objects;
		Vector3 m_cursorPosition;

		Vector3 m_beginPosition = { 0.0f, 0.0f, 0.0f };
		Vector3 m_beginRotation = { 0.0f, 0.0f, 0.0f };
		Vector3 m_beginScale = { 1.0f, 1.0f, 1.0f };
	};
}
