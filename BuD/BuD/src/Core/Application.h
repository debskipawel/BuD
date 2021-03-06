#pragma once

#include <memory>
#include <map>

#include "GuiLayer.h"
#include "KeyboardKeys.h"
#include "InteractionMode.h"

#include "Event/IEventDispatchable.h"
#include "DirectX11/DX11Renderer.h"
#include "GUI/ObjectsEditor.h"
#include "Win32/Win32Window.h"

namespace BuD
{
	class Application : public IEventDispatchable
	{
	public:
		int Run(HINSTANCE hInstance);

		void OnUpdate();
		void Render();

		void OnConcreteEvent(WindowClosedEvent& e) override;
		void OnConcreteEvent(WindowResizedEvent& e) override;
		void OnConcreteEvent(WindowEnterSizeMoveEvent& e) override;
		void OnConcreteEvent(WindowExitSizeMoveEvent& e) override;
		
		void OnConcreteEvent(KeyDownEvent& e);
		void OnConcreteEvent(KeyReleaseEvent& e);
		
		void OnConcreteEvent(MouseButtonDownEvent& e);
		void OnConcreteEvent(MouseButtonReleasedEvent& e);
		void OnConcreteEvent(MouseMovedEvent& e);

		void OnConcreteEvent(ToggleFullscreenEvent& e) override;

	private:
		Application() = default;
		Application(const Application& other) = delete;
		Application operator=(const Application& other) = delete;
		
		void HandleControls(float deltatime);

		static std::shared_ptr<Application> s_app;

		RenderingMode m_selectedMode;

		std::unique_ptr<DX11Device> m_device;
		std::shared_ptr<DX11Renderer> m_renderer;
		std::shared_ptr<Win32Window> m_window;

		std::unique_ptr<GuiLayer> m_guiLayer;
		std::unique_ptr<ObjectsEditor> m_guiEditor;

		std::map<KeyboardKeys, bool> m_keyMap;
		std::shared_ptr<Mesh> m_pointMesh;

		Scene m_scene;

		dxm::Vector3 m_cursorPosition = { 0.0f, 0.0f, 0.0f };
		LARGE_INTEGER m_counterStart, m_freq;

		int m_prevX, m_prevY;

		bool m_cameraMoving = false;
		bool m_shouldRun = true;
		
		InteractionMode m_appMode = InteractionMode::NONE;
		bool m_inAction = false;
		Vector3 m_prevActionPoint;

		bool m_in_sizemove = false;
		bool m_minimized = false;

		bool m_inPreviewMode = false;

	public:
		inline static std::shared_ptr<Application> Get()
		{
			if (!s_app)
			{
				s_app = std::shared_ptr<Application>(new Application());
			}

			return s_app;
		}

		inline void OnEvent(Event& e) override
		{
			e.Visit(*this);
		}
	};
}
