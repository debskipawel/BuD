#include "Application.h"

#include <stdio.h>
#include <windows.h>

#include "../Window/Win32Window.h"
#include "ApplicationInfo.h"

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run()
	{
        //m_clientApp = CreateClientApp();

        auto window = std::make_shared<Win32Window>(ApplicationInfo());//m_clientApp->GetWindow();

        window->Show();

        while (m_shouldRun)
        {
            if (window->EventQueueNotEmpty())
            {
                window->HandleEvent();
            }
            else
            {
                this->OnUpdate();
            }
        }

        return 0;
	}

    void Application::OnUpdate()
    {

    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        m_shouldRun = false;
    }
}