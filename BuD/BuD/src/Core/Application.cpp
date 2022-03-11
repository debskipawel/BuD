#include "Application.h"

#include <stdio.h>
#include <windows.h>

#include "../Event/WindowEvents.h"

#include "../Win32/Win32Window.h"
#include "ApplicationInfo.h"

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run()
	{
        m_clientApp = CreateClientApp();

        auto window = m_clientApp->GetWindow();

        window->Show();

        while (true)
        {
            window->ProcessEvents();

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
        auto renderer = m_clientApp->GetRenderer();

        renderer->Begin();
        renderer->End();
    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}