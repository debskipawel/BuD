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

		friend class DX11Renderer;

	private:
		HWND m_hwnd;
	};
}

#endif
