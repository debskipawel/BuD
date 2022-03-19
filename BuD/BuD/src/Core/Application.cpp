#include "Application.h"

#include "Event/WindowEvents.h"

#include "Win32/Win32Window.h"
#include "ApplicationInfo.h"
#include "Geometry/SceneObject.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <stdio.h>
#include <windows.h>
#include <algorithm>

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run(HINSTANCE hInstance)
	{
        m_window = std::make_shared<Win32Window>(ApplicationInfo(), hInstance);
        m_renderer = std::make_shared<DX11Renderer>(m_window);
        m_clientApp = CreateClientApp(m_renderer->Device());

        m_guiLayer = std::make_unique<GuiLayer>(m_renderer, m_window);

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

        auto camera = m_clientApp->GetCamera();

        for (auto& [id, entity] : SceneObject::GetAll())
        {
            m_renderer->Draw(entity->GetModel(), camera);
        }

        m_guiLayer->BeginFrame();
        m_clientApp->OnGuiRender();
        m_guiLayer->EndFrame();

        m_renderer->End();
    }

    void Application::OnConcreteEvent(WindowResizedEvent& e)
    {
        if (e.m_minimized)
        {
            if (!m_minimized)
            {
                m_minimized = true;
            }
        }
        else if (m_minimized)
        {
            m_minimized = false;
        }

        if (!m_in_sizemove)
        {
            m_renderer->UpdateBuffersSize(e.m_width, e.m_height);
        }

        m_clientApp->GetCamera()->UpdateAspectRatio(static_cast<float>(e.m_width) / e.m_height);
    }

    void Application::OnConcreteEvent(WindowEnterSizeMoveEvent& e)
    {
        m_in_sizemove = true;
    }

    void Application::OnConcreteEvent(WindowExitSizeMoveEvent& e)
    {
        m_in_sizemove = false;

        m_renderer->UpdateBuffersSize(m_window->Width(), m_window->Height());
    }

    void Application::OnConcreteEvent(ToggleFullscreenEvent& e)
    {
        m_window->ToggleFullscreen();
    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}