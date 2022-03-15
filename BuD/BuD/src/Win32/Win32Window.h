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
		Win32Window(const ApplicationInfo& appInfo, HINSTANCE hInstance);
		virtual ~Win32Window() = default;

		virtual void Show() override;
		virtual void ProcessEvents() override;

		virtual int Width() override;
		virtual int Height() override;

		virtual void ToggleFullscreen() override;

		inline HWND Handle() { return m_hwnd; }

	private:
		HWND m_hwnd;
		HINSTANCE m_hInstance;
	};
}

#endif
