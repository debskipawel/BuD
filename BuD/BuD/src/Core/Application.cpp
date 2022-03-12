#include "Application.h"

#include <stdio.h>
#include <windows.h>

#include "../Event/WindowEvents.h"

#include "../Win32/Win32Window.h"
#include "ApplicationInfo.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run()
	{
        m_clientApp = CreateClientApp();
        m_window = std::make_shared<Win32Window>(ApplicationInfo());
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
        auto cube = RenderableSceneEntity::Cube(m_renderer->Device().Raw());

        m_renderer->Begin();
        m_renderer->Draw(cube);
        m_renderer->End();
    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}