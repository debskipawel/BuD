#ifdef _WIN32
#pragma once

#include <memory>
#include <map>
#include <Windows.h>

#include "../Core/KeyboardKeys.h"
#include "../Core/MouseCode.h"

#include "../Event/Event.h"

namespace BuD
{
	class Win32EventFactory
	{
	public:
		std::unique_ptr<Event> Construct(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static Win32EventFactory& Get()
		{
			if (!instance)
			{
				instance = std::unique_ptr<Win32EventFactory>{ new Win32EventFactory() };
			}

			return *instance;
		}

		Win32EventFactory(Win32EventFactory& other) = delete;
		void operator=(const Win32EventFactory&) = delete;

	protected:
		Win32EventFactory();

		static std::unique_ptr<Win32EventFactory> instance;

		std::map<int, MouseCode> m_mouseCodeMap;
	};
}

#endif
