#pragma once

#include <memory>

#include "ClientApp.h"
#include "GuiLayer.h"

#include "Event/IEventDispatchable.h"
#include "DirectX11/DX11Renderer.h"
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

		void OnConcreteEvent(ToggleFullscreenEvent& e) override;

	private:
		Application() = default;
		Application(const Application& other) = delete;
		Application operator=(const Application& other) = delete;
		
		static std::shared_ptr<Application> s_app;

		std::shared_ptr<ClientApp> m_clientApp;

		std::shared_ptr<DX11Renderer> m_renderer;
		std::shared_ptr<Win32Window> m_window;

		std::unique_ptr<GuiLayer> m_guiLayer;

		bool m_shouldRun = true;

		bool m_in_sizemove = false;
		bool m_minimized = false;

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
			
			if (!e.m_handled)
			{
				m_clientApp->OnEvent(e);
			}
		}
	};
}
