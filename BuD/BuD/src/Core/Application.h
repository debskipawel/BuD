#pragma once

#include <memory>

#include "ClientApp.h"

#include "../Event/IEventDispatchable.h"
#include "DirectX11/DX11Renderer.h"
#include "Win32/Win32Window.h"

namespace BuD
{
	class Application : public IEventDispatchable
	{
	public:
		int Run();

		void OnUpdate();
		void Render();

		void OnConcreteEvent(WindowClosedEvent& e) override;

	private:
		Application() = default;
		Application(const Application& other) = delete;
		Application operator=(const Application& other) = delete;
		
		static std::shared_ptr<Application> s_app;

		std::shared_ptr<ClientApp> m_clientApp;
		std::shared_ptr<DX11Renderer> m_renderer;
		std::shared_ptr<Win32Window> m_window;

		std::unique_ptr<RenderableSceneEntity> entity;

		bool m_shouldRun = true;

		friend class ClientApp;

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
