#include "Application.h"

#include "Event/WindowEvents.h"

#include "Win32/Win32Window.h"
#include "ApplicationInfo.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <stdio.h>
#include <windows.h>

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run(HINSTANCE hInstance)
	{
        m_clientApp = CreateClientApp();
        m_window = std::make_shared<Win32Window>(ApplicationInfo(), hInstance);
        m_renderer = std::make_shared<DX11Renderer>(m_window);

        m_window->Show();

        while (true)
        {
            m_window->ProcessEvents();

            if (!m_shouldRun)
            {
                break;
            }

            this->OnUpdate();
            this->Render();
        }

        return 0;
	}

    void Application::OnUpdate()
    {
        this->m_clientApp->OnUpdate();
    }

    void Application::Render()
    {
        m_renderer->Begin();

        // draw all models

        m_renderer->End();
    }

    void Application::OnConcreteEvent(WindowResizedEvent& e)
    {
        m_renderer->UpdateBuffersSize(e.m_width, e.m_height);
    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}