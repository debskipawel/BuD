#include "Application.h"

#include "ApplicationInfo.h"
#include "Camera/CameraFactory.h"
#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"
#include "DirectX11/DX11StereoscopicRenderer.h"

#include "Event/WindowEvents.h"
#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"

#include <Objects/Abstract/PointBasedObject.h>
#include <Objects/SceneObjectGroup.h>

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
        m_window = std::make_shared<Win32Window>(ApplicationInfo(), hInstance);
        m_device = std::make_unique<DX11Device>(m_window);
        m_guiEditor = std::make_unique<ObjectsEditor>(m_scene, m_window);

        m_selectedMode = m_guiEditor->GetRenderingMode();

        switch (m_selectedMode)
        {
            case RenderingMode::STANDARD:
            {
                m_renderer = std::make_shared<DX11Renderer>(*m_device, m_window);
                break;
            }
            case RenderingMode::ANAGLIPH:
            {
                m_renderer = std::make_shared<DX11StereoscopicRenderer>(*m_device, m_window);
                break;
            }
        }

        m_guiLayer = std::make_unique<GuiLayer>(m_renderer, m_window);

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

        auto prevMode = m_selectedMode;
        m_selectedMode = m_guiEditor->GetRenderingMode();

        if (m_selectedMode != prevMode)
        {
            switch (m_selectedMode)
            {
                case RenderingMode::STANDARD:
                {
                    m_renderer = std::make_shared<DX11Renderer>(*m_device, m_window);
                    break;
                }
                case RenderingMode::ANAGLIPH:
                {
                    m_renderer = std::make_shared<DX11StereoscopicRenderer>(*m_device, m_window);
                    break;
                }
            }

            m_guiLayer->UpdateRenderer(m_renderer);
        }
    }

    void Application::Render()
    {
        m_renderer->Begin();
        auto camera = m_guiEditor->GetCamera();

        auto sceneObjects = m_scene.GetAllSceneObjects();

        for (auto& [id, object] : sceneObjects)
        {
            for (uint32_t index = 0; index < object->MeshesCount(); index++)
            {
                m_renderer->Draw(object->GetMesh(index), camera, id);
            }

            if ((object->GetType() & ObjectType::POINT_BASED) != ObjectType::NONE)
            {
                auto controlPoints = reinterpret_cast<PointBasedObject*>(object.get())->GetVirtualPoints();

                for (auto& controlPoint : controlPoints)
                {
                    m_pointMesh->m_position = controlPoint;
                    m_renderer->Draw(m_pointMesh, camera);
                }
            }
        }

        m_renderer->Draw(Cursor::GetCursorAt(m_guiEditor->CursorPosition(), m_renderer->Device())->GetMesh(), camera);

        auto selected = SceneObjectsGroup(m_scene.GetAllSelected());

        if (selected.Count() > 0)
        {
            auto cursor = Cursor::GetCursorAt(selected.Centroid(), m_renderer->Device());
            m_renderer->Draw(cursor->GetMesh(), camera);
        }

        m_renderer->End();

        if (!m_inDebug)
        {
            m_guiLayer->BeginFrame();
            m_guiEditor->DrawGui(m_renderer->Device());
            m_guiLayer->EndFrame();
        }

        m_renderer->Present();
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

        m_guiEditor->GetCamera()->UpdateViewport(e.m_width, e.m_height);
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
                auto objects = m_scene.GetAllSelected();

                for (auto& [id, obj] : objects)
                {
                    m_scene.RemoveSceneObject(id);
                }

                break;
            }
            case KeyboardKeys::Escape:
            {
                auto objects = m_scene.GetAllSelected();

                for (auto& [id, obj] : objects)
                {
                    obj->OnUnselect();
                }

                break;
            }
            case KeyboardKeys::D1:
            {
                m_appMode = InteractionMode::TRANSLATION;
                break;
            }
            case KeyboardKeys::D2:
            {
                m_appMode = InteractionMode::ROTATION;
                break;
            }
            case KeyboardKeys::D3:
            {
                m_appMode = InteractionMode::SCALE;
                break;
            }
            case KeyboardKeys::D0:
            {
                m_inDebug = !m_inDebug;
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
            auto selected = SceneObjectsGroup(m_scene.GetAllSelected());

            m_inAction = true;

            m_prevX = e.m_xPos;
            m_prevY = e.m_yPos;
            m_prevActionPoint = m_guiEditor->GetCamera()->MoveWorldPointToPixels(selected.Centroid(), e.m_xPos, e.m_yPos);

            auto id = m_renderer->GetObjectFrom(e.m_xPos, e.m_yPos);
            
            auto object = m_scene.GetSceneObject(id);

            if (object)
            {
                object->Selected() ? object->OnUnselect() : object->OnSelect();

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
        else if (e.m_button == MouseCode::LEFT)
        {
            m_inAction = false;
        }
    }

    void Application::OnConcreteEvent(MouseMovedEvent& e)
    {
        auto camera = m_guiEditor->GetCamera();

        m_prevX += e.m_xOffset;
        m_prevY += e.m_yOffset;

        if (m_cameraMoving)
        {
            camera->ProcessMouseMovement(e.m_xOffset, e.m_yOffset);
        }

        if (m_inAction)
        {
            auto selected = SceneObjectsGroup(m_scene.GetAllSelected());

            auto diff = camera->MoveWorldPointToPixels(selected.Centroid(), m_prevX, m_prevY) - m_prevActionPoint;
            
            if (diff.x != diff.x || diff.y != diff.y || diff.z != diff.z)
            {
                return;
            }

            switch (m_appMode)
            {
                case InteractionMode::ROTATION:
                {
                    selected.RotateAroundCentroid(diff * 5.0f);
                    break;
                }
                case InteractionMode::SCALE:
                {
                    selected.ScaleAroundCentroid(diff * 0.5f + Vector3(1.0f, 1.0f, 1.0f));
                    break;
                }
                case InteractionMode::TRANSLATION:
                {
                    selected.MoveAll(diff);
                    break;
                }
                default:
                {
                    break;
                }
            }

            m_prevActionPoint += diff;
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

        auto camera = m_guiEditor->GetCamera();

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

        camera->Move(5.0f * deltatime * Vector3{ dx, dy, dz });
    }

    void Application::OnConcreteEvent(WindowClosedEvent& e)
    {
        e.m_handled = true;
        m_shouldRun = false;
    }
}