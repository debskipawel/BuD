#pragma once

#include <SimpleMath.h>

#include "Core/RenderingMode.h"

#include <Objects/Scene.h>

#include <Visitors/GuiDrawer.h>
#include <GUI/ApplicationSettings.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class ObjectsEditor
	{
	public:
		ObjectsEditor(std::shared_ptr<Win32Window> window);

		std::shared_ptr<AbstractCamera> GetCamera();

		Scene& GetScene() { return m_scene; }
		ApplicationSettings& GetSettings() { return m_settings; }

		Vector3 CursorPosition() const { return m_cursorPosition; }
		void DrawGui(const DX11Device& device);
		void SetCursorTo(int pixelX, int pixelY);
		void SelectionChanged();

		const RenderingMode& GetRenderingMode() const { return m_selectedType; }

	protected:
		
		void DrawMainSettings(const DX11Device& device);

		void DrawContextMenu(const DX11Device& device);

		void DrawSelectableList(const DX11Device& device);
		void DrawSelectableConfig(const DX11Device& device);
		bool DrawSurfacePopup(std::string title);

		void UpdateCamera(const RenderingMode& selectedType);
		void GuiForCamera(const RenderingMode& cameraType, std::string name);

		std::shared_ptr<AbstractCamera> m_activeCamera;
		std::shared_ptr<Win32Window> m_window;

		SceneObject* m_selectedObject = nullptr;
		
		Scene m_scene;
		GuiDrawer m_guiDrawer;
		ApplicationSettings m_settings;
		
		Vector3 m_cursorPosition;

		Vector3 m_beginPosition = { 0.0f, 0.0f, 0.0f };
		Vector3 m_beginRotation = { 0.0f, 0.0f, 0.0f };
		Vector3 m_beginScale = { 1.0f, 1.0f, 1.0f };

		RenderingMode m_selectedType;
	};
}
