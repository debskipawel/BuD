#include "Application.h"

#include <stdio.h>
#include <windows.h>

#include "../Event/WindowEvents.h"

#include "../OpenGL/GLFWWindow.h"
#include "../Win32/Win32Window.h"
#include "ApplicationInfo.h"

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run()
	{
        //m_clientApp = CreateClientApp();

        auto window = std::make_shared<GLFWWindow>(ApplicationInfo());//m_clientApp->GetWindow();

        window->Show();

        while (true)
        {
            window->ProcessEvents();

            if (!m_shouldRun)
            {
                break;
            }

            this->OnUpdate();
        }

        return 0;
	}

    void Application::OnUpdate()
    {

    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}