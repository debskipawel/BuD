#ifdef _WIN32

#include "Win32EventFactory.h"

#include "../Event/KeyboardEvents.h"
#include "../Event/MouseEvents.h"
#include "../Event/WindowEvents.h"

#include <windowsx.h>

namespace BuD
{
#define MAP_INSERT(map, key, value) map.insert(std::make_pair(key, value))

	std::unique_ptr<Win32EventFactory> Win32EventFactory::instance = nullptr;

	Win32EventFactory::Win32EventFactory()
	{
		MAP_INSERT(m_mouseCodeMap, WM_LBUTTONDOWN, MouseCode::LEFT);
		MAP_INSERT(m_mouseCodeMap, WM_MBUTTONDOWN, MouseCode::MIDDLE);
		MAP_INSERT(m_mouseCodeMap, WM_RBUTTONDOWN, MouseCode::RIGHT);
		MAP_INSERT(m_mouseCodeMap, WM_LBUTTONUP, MouseCode::LEFT);
		MAP_INSERT(m_mouseCodeMap, WM_MBUTTONUP, MouseCode::MIDDLE);
		MAP_INSERT(m_mouseCodeMap, WM_RBUTTONUP, MouseCode::RIGHT);
	}

	std::unique_ptr<Event> Win32EventFactory::Construct(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			// ----- KEYBOARD EVENTS -----
			case WM_KEYDOWN:
			{
				return std::make_unique<KeyDownEvent>(static_cast<KeyboardKeys>(wParam));
			}
			case WM_KEYUP:
			{
				return std::make_unique<KeyReleaseEvent>(static_cast<KeyboardKeys>(wParam));
			}
			// ----- MOUSE EVENTS -----
			case WM_INPUT:
			{
				return std::make_unique<MouseRawInputEvent>();
			}
			case WM_MOUSEMOVE:
			{
				return std::make_unique<MouseMovedEvent>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			{
				auto key = (*m_mouseCodeMap.find(message)).second;
				return std::make_unique<MouseButtonDownEvent>(key);
			}
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			{
				auto key = (*m_mouseCodeMap.find(message)).second;
				return std::make_unique<MouseButtonReleasedEvent>(key);
			}
			case WM_MOUSEWHEEL:
			{
				return std::make_unique<MouseScrolledEvent>();
			}
			// ----- WINDOW EVENTS -----
			case WM_ACTIVATEAPP:
			{
				return std::make_unique<ActivateAppEvent>(wParam);
			}
			case WM_MOVE:
			{
				int xPos = (int)(short)LOWORD(lParam);
				int yPos = (int)(short)HIWORD(lParam);

				return std::make_unique<WindowMovedEvent>(xPos, yPos);
			}
			case WM_ENTERSIZEMOVE:
			{
				return std::make_unique<WindowEnterSizeMoveEvent>();
			}
			case WM_EXITSIZEMOVE:
			{
				return std::make_unique<WindowExitSizeMoveEvent>();
			}
			case WM_DESTROY:
			{
				return std::make_unique<WindowClosedEvent>();
			}
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);

				return std::make_unique<WindowResizedEvent>(width, height);
			}
			case WM_SYSKEYDOWN:
			{
				if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
				{
					return std::make_unique<ToggleFullscreenEvent>();
				}
			}
		}

		return nullptr;
	}
}

#endif
