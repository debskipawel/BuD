#pragma once

#ifdef _WIN32

#include "Window.h"

#include <Windows.h>

#include "../Core/ApplicationInfo.h"

namespace BuD
{
	class Win32Window : public Window
	{
	public: 
		Win32Window(const ApplicationInfo& appInfo);
		virtual ~Win32Window() = default;

		virtual void Show() override;
		virtual bool EventQueueNotEmpty() override;
		virtual void HandleEvent() override;

	private:
		HWND m_hwnd;
		MSG m_msg;

		bool m_messageRead = false;
	};
}

#endif
