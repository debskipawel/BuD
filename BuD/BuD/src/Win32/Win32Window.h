#pragma once

#ifdef _WIN32

#include "../Core/Window.h"
#include "../Core/ApplicationInfo.h"

#include <Windows.h>

namespace BuD
{
	class Win32Window : public Window
	{
	public: 
		Win32Window(const ApplicationInfo& appInfo);
		virtual ~Win32Window() = default;

		virtual void Show() override;
		virtual void ProcessEvents() override;

	private:
		HWND m_hwnd;
		MSG m_msg;

		bool m_messageRead = false;
	};
}

#endif
