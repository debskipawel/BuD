#include "Application.h"

#include "ApplicationInfo.h"
#include "Camera/CameraFactory.h"
#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include "Event/WindowEvents.h"
#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"

#include "Geometry/SceneObject.h"
#include "Geometry/Point.h"
#include "Geometry/ObjectsCollection.h"

#include "Scene/Cursor.h"
#include "Win32/Win32Window.h"

#include <stdio.h>
#include <windows.h>
#include <algorithm>

#include <imgui.h>

namespace BuD
{
    std::shared_ptr<Application> Application::s_app = nullptr;

	int Application::Run(HINSTANCE hInstance)
	{
        auto collection = std::vector<std::shared_ptr<SceneObject>>();

        m_window = std::make_shared<Win32Window>(ApplicationInfo(), hInstance);
        m_renderer = std::make_shared<DX11Renderer>(m_window);
        m_camera = CameraFactory::MakePerspective(Vector3(0.0f, 0.0f, 3.0f), Vector3(0.0f, 0.0f, -1.0f));

        m_guiLayer = std::make_unique<GuiLayer>(m_renderer, m_window);
        m_guiEditor = std::make_unique<ObjectsEditor>(collection, m_camera, m_window);

        m_pointMesh = Point::GetMesh(m_renderer->Device());

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

        HandleControls(deltaTime);
    }

    void Application::Render()
    {
        m_renderer->Begin();

        for (auto& [id, entity] : SceneObject::GetAll())
        {
            for (uint32_t index = 0; index < entity->MeshesCount(); index++)
            {
                m_renderer->Draw(entity->GetMesh(index), m_camera, id);
            }

            auto controlPoints = entity->VirtualControlPoints();

            for (auto& controlPoint : controlPoints)
            {
                m_pointMesh->m_position = controlPoint;
                m_renderer->Draw(m_pointMesh, m_camera);
            }
        }

        m_renderer->Draw(Cursor::GetCursorAt(m_guiEditor->CursorPosition(), m_renderer->Device())->GetMesh(), m_camera);

        if (SceneObject::GetSelected().Count() > 0)
        {
            auto cursor = Cursor::GetCursorAt(SceneObject::GetSelected().Centroid(), m_renderer->Device());
            m_renderer->Draw(cursor->GetMesh(), m_camera);
        }

        m_guiLayer->BeginFrame();
        m_guiEditor->DrawGui(m_renderer->Device());
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

        m_camera->UpdateViewport(e.m_width, e.m_height);
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

    void Application::OnConcreteEvent(KeyDownEvent& e)
    {
        m_keyMap[e.m_key] = true;

        switch (e.m_key)
        {
            case KeyboardKeys::Delete:
            {
                auto& objects = SceneObject::GetSelected().Objects();
                std::vector<uint32_t> selectedIds(objects.size());

                std::transform(objects.begin(), objects.end(), selectedIds.begin(), [](SceneObject* obj) { return obj->Id(); });

                for (auto id : selectedIds)
                {
                    SceneObject::DeleteObject(id);
                }

                break;
            }
            case KeyboardKeys::Escape:
            {
                auto& objects = SceneObject::GetSelected().Objects();
                std::vector<uint32_t> selectedIds(objects.size());

                std::transform(objects.begin(), objects.end(), selectedIds.begin(), [](SceneObject* obj) { return obj->Id(); });

                for (auto id : selectedIds)
                {
                    SceneObject::Get(id)->Unselect();
                }

                break;
            }
        }
    }

    void Application::OnConcreteEvent(KeyReleaseEvent& e)
    {
        m_keyMap[e.m_key] = false;
    }

    void Application::OnConcreteEvent(MouseButtonDownEvent& e)
    {
        if (e.m_button == MouseCode::RIGHT)
        {
            m_cameraMoving = true;
        }
        else if (e.m_button == MouseCode::LEFT)
        {
            auto r = m_renderer->GetObjectFrom(e.m_xPos, e.m_yPos);
            
            auto object = SceneObject::Get(r);

            if (object)
            {
                object->IsSelected() ? object->Unselect() : object->Select();

                m_guiEditor->SelectionChanged();
            }
            else
            {
                m_guiEditor->SetCursorTo(e.m_xPos, e.m_yPos);
            }
        }
    }

    void Application::OnConcreteEvent(MouseButtonReleasedEvent& e)
    {
        if (e.m_button == MouseCode::RIGHT)
        {
            m_cameraMoving = false;
        }
    }

    void Application::OnConcreteEvent(MouseMovedEvent& e)
    {
        if (m_cameraMoving)
        {
            m_camera->ProcessMouseMovement(e.m_xOffset, e.m_yOffset);
        }
    }

    void Application::OnConcreteEvent(ToggleFullscreenEvent& e)
    {
        m_window->ToggleFullscreen();
    }

    void Application::HandleControls(float deltatime)
    {
        if (!m_cameraMoving)
        {
            return;
        }

        float dx = 0.0f, dy = 0.0f, dz = 0.0f;

        if (m_keyMap[BuD::KeyboardKeys::W])
        {
            dz += 1.0f;
        }

        if (m_keyMap[BuD::KeyboardKeys::S])
        {
            dz -= 1.0f;
        }

        if (m_keyMap[BuD::KeyboardKeys::A])
        {
            dx -= 1.0f;
        }

        if (m_keyMap[BuD::KeyboardKeys::D])
        {
            dx += 1.0f;
        }

        if (m_keyMap[BuD::KeyboardKeys::Q])
        {
            // up
            dy += 1.0f;
        }

        if (m_keyMap[BuD::KeyboardKeys::E])
        {
            // down
            dy -= 1.0f;
        }

        m_camera->Move(5.0f * deltatime * Vector3{ dx, dy, dz });
    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}