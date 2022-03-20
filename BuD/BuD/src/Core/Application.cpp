#include "Application.h"

#include "Event/WindowEvents.h"

#include "Win32/Win32Window.h"
#include "ApplicationInfo.h"
#include "Geometry/SceneObject.h"
#include "Scene/Cursor.h"

#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include <stdio.h>
#include <windows.h>
#include <algorithm>

#include <imgui.h>

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run(HINSTANCE hInstance)
	{
        m_window = std::make_shared<Win32Window>(ApplicationInfo(), hInstance);
        m_renderer = std::make_shared<DX11Renderer>(m_window);
        m_clientApp = CreateClientApp(m_renderer->Device());

        m_guiLayer = std::make_unique<GuiLayer>(m_renderer, m_window);

        QueryPerformanceCounter(&m_counterStart);
        QueryPerformanceFrequency(&m_freq);

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
        LARGE_INTEGER a;
        QueryPerformanceCounter(&a);
        float deltaTime = static_cast<float>(a.QuadPart - m_counterStart.QuadPart) / m_freq.QuadPart;
        m_counterStart = a;

        this->m_clientApp->OnUpdate(deltaTime);
    }

    void Application::Render()
    {
        m_renderer->Begin();

        auto camera = m_clientApp->GetCamera();

        for (auto& [id, entity] : SceneObject::GetAll())
        {
            m_renderer->Draw(entity->GetModel(), camera);
        }

        m_renderer->Draw(Cursor::GetCursorAt(m_cursorPosition, m_renderer->Device())->GetModel(), camera);

        m_guiLayer->BeginFrame();

        ImGui::Begin("Main settings");
        ImGui::DragFloat("x", &m_cursorPosition.x);
        ImGui::DragFloat("y", &m_cursorPosition.y);
        ImGui::DragFloat("z", &m_cursorPosition.z);


        ImGui::End();

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